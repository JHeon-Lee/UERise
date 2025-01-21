// Fill out your copyright notice in the Description page of Project Settings.

#include "MH_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CharacterStat/UtusiStatComponent.h"
#include "Animation/MHAnimInstancePlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/DataTable.h"
#include "Engine/DamageEvents.h"
#include "MHGameInstance.h"
#include "UI/UtusiHPBarWidget.h"
#include "UI/MHWidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/TimelineComponent.h"
#include "Character/Component/MH_PlayerStatComponent.h"
#include "Character/Component/MHGreatSwordComponent.h"
#include "Character/Component/MHValutComponent.h"
#include "Character/Component/MHCharacterMovementComponent.h"
#include "Character/Component/MHEquipmentComponent.h"
#include "GameData/MHPlayerAttackData.h"
#include "DamageType/MHBasicDamageType.h"
#include "LegacyCameraShake.h"


AMH_PlayerCharacter::AMH_PlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UMHCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	// KeyArray Initialize
	for (uint8 Enum = static_cast<uint8>(EKeyInfo::Default); Enum < static_cast<uint8>(EKeyInfo::Max); Enum++)
	{
		EKeyInfo key = static_cast<EKeyInfo>(Enum);
		KeyArray.Add(key, false);
	}
	// KeyButton Initialize
	for (uint8 Enum = static_cast<uint8>(EButtons::Default); Enum < static_cast<uint8>(EButtons::Max); Enum++)
	{
		EButtons key = static_cast<EButtons>(Enum);
		PressMap.Add(key, false);
	}


	ComponentAttach();
}

void AMH_PlayerCharacter::ComponentAttach()
{
	if (UMHCharacterMovementComponent* MovementComponent = Cast<UMHCharacterMovementComponent>(GetCharacterMovement()))
	{
		MovementComponent->OnMovementFallingDelegate.AddUObject(this, &AMH_PlayerCharacter::MakeFalling);
		MovementComponent->OnMovementWalkingDelegate.AddUObject(this, &AMH_PlayerCharacter::KeyPressCheck);
	}

	// Valut Component
	PlayerValutComponent = CreateDefaultSubobject<UMHValutComponent>(TEXT("ValutComponent"));
	if (PlayerValutComponent)
	{
		PlayerValutComponent->CanPlayMontageDelegate.AddUObject(this, &AMH_PlayerCharacter::PlayValutMontage);
	}

	//Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 450.0f;
	CameraBoom->TargetOffset = FVector(0, 0, 70);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	EquipmentComponent = CreateDefaultSubobject<UMHEquipmentComponent>(TEXT("EquipmentComponent"));

	// Mesh Attach
	WireBug = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WireBug"));
	WireBug->SetupAttachment(GetMesh());

	BuffEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BuffEffect"));
	BuffEffect->SetupAttachment(GetMesh(), TEXT("R_Grip_00"));

	BugEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BugEffect"));
	BugEffect->SetupAttachment(WireBug);

	// Stat Component
	StatComponent = CreateDefaultSubobject<UMH_PlayerStatComponent>(TEXT("StatComponent"));
	
	// WidgetComponent
	PlayerWidgetComponent = CreateDefaultSubobject<UMHWidgetComponent>(TEXT("Widget"));
	PlayerWidgetComponent->SetupAttachment(GetMesh());
	PlayerWidgetComponent->SetRelativeLocation(FVector(0, 0, 180));
	static ConstructorHelpers::FClassFinder<UUtusiHPBarWidget> HpBarWidgetRef(TEXT("/Game/Characters/Utusi/UI/WBP_HpProgressBar.WBP_HpProgressBar_C"));
	if (HpBarWidgetRef.Class)
	{
		PlayerWidgetComponent->SetWidgetClass(HpBarWidgetRef.Class);
		PlayerWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		PlayerWidgetComponent->SetDrawSize(FVector2D(150.0f, 15.0f));
		PlayerWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Log, TEXT("PlayerWidgetComponent Construct Success"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerWidgetComponent Construct Failed"));
	}
	
}


void AMH_PlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
//	Stat->OnHpZero.AddUObject(this, &)
}

// Called when the game starts or when spawned
void AMH_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("PlayerBeginPlay Called"));
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(UtusiMappingContext, 0);
	}	

}

void AMH_PlayerCharacter::URotate(const FInputActionValue& Value)
{
	ActionValue = Value.Get<FVector2D>();
	PressWASD = true;

	FVector vector(ActionValue.X, -1 * ActionValue.Y, 0.0);
	CurrentTrgtZ = vector.ToOrientationRotator().Yaw + FollowCamera->GetComponentRotation().Yaw;

	KeyDir = CurrentTrgtZ - GetActorRotation().Yaw;

	if (KeyDir > 180.0f)
	{
		KeyDir = KeyDir - 360.0f;
	}
	else if (KeyDir < -180.0f)
	{
		KeyDir = KeyDir + 360.0f;
	}
	
	if (KeyDir > -45.0f && KeyDir <= 45.0f)	KeyDirInt = 0;
	else if (KeyDir > -135.0f && KeyDir <= -45.0f)	KeyDirInt = 3;
	else if (KeyDir > 45.0f && KeyDir <= 135.0f)	KeyDirInt = 1;
	else											KeyDirInt = 2;
}

void AMH_PlayerCharacter::ULook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	CameraBoom->AddRelativeRotation(FRotator(-1 * LookAxisVector.Y, LookAxisVector.X, 0));
	FRotator BoomRotate = CameraBoom->GetRelativeRotation();	

	BoomRotate.Pitch = UKismetMathLibrary::ClampAngle(BoomRotate.Pitch, -70, 70);
	
	CameraBoom->SetRelativeRotation(BoomRotate);
}

void AMH_PlayerCharacter::ComboStartA()
{	
	PressA = true;

	// return, if any Montage Playing now
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || !GetCharacterMovement()->IsMovingOnGround())
	{
		return;
	}

	if (UMHGreatSwordComponent* GreatSwordComponent = FindComponentByClass<UMHGreatSwordComponent>())
	{
        if (WeaponType == EWeaponType::GreatSwdArmed)
		{
			PlayAnimMontage(FIND_MONTAGE("Character.Player.Montage.A_GSwd"));
		}		
	}

	if (WeaponType == EWeaponType::Unarmed)
	{
		PlayAnimMontage(FIND_MONTAGE("Character.Player.Montage.A_Unarmed"));	
	}

}

void AMH_PlayerCharacter::ComboStartY()
{
	PressYOn();

	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}

	if (PressLT && CurWireBugStack > 0)
	{
		PlayAnimMontage(FIND_MONTAGE("Character.Player.Montage.Y+LT_Unarmed"));
		return;
	}

	if (UMHGreatSwordComponent* GreatSwordComponent = FindComponentByClass<UMHGreatSwordComponent>())
	{
		GreatSwordComponent->ComboStartY(PressMap, WeaponType, true);
	}



}

void AMH_PlayerCharacter::ComboStartRT()
{
	PressRTOn();

	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}

	if (UMHGreatSwordComponent* GreatSwordComponent = FindComponentByClass<UMHGreatSwordComponent>())
	{		
		PlayAnimMontage(FIND_MONTAGE("Character.Player.Montage.RT"));
	}

}

void AMH_PlayerCharacter::ComboStartB()
{
	PressBOn();

	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}

	if (UMHGreatSwordComponent* GreatSwordComponent = FindComponentByClass<UMHGreatSwordComponent>())
	{
		PlayAnimMontage(FIND_MONTAGE("Character.Player.Montage.B_Gswd"));
		
	}

	// Near Prop Get
	if (IsValid(NearPropRef) && WeaponType == EWeaponType::Unarmed)
	{
		double X = NearPropRef->GetActorLocation().X - GetActorLocation().X;
		double Y = NearPropRef->GetActorLocation().Y - GetActorLocation().Y;

		FRotator Rot = FVector(X, Y, 0.0).ToOrientationRotator();
		Rot.Yaw -= 90.0;

		SetActorRotation(Rot);
	}
}

// Called every frame
void AMH_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	KeyPressCheck();

	BuffTimer += GetWorld()->DeltaTimeSeconds;

	OpacityFloatTimeline.TickTimeline(DeltaTime);
}

void AMH_PlayerCharacter::KeyPressCheck()
{
	KeyArray[EKeyInfo::Default] = true;
	KeyArray[EKeyInfo::LT_Y] = PressLT && PressY && (CurWireBugStack > 0 ? true : false);
	KeyArray[EKeyInfo::LT_B] = PressLT && PressB && (CurWireBugStack > 0 ? true : false);
	KeyArray[EKeyInfo::Y_B] = PressY && PressB;
	KeyArray[EKeyInfo::WASD_Y] = PressY && PressWASD;
	KeyArray[EKeyInfo::Y] = PressY;
	KeyArray[EKeyInfo::Release_Y] = !PressY;
	KeyArray[EKeyInfo::B] = PressB;
	KeyArray[EKeyInfo::Release_B] = !PressB;
	KeyArray[EKeyInfo::Space] = PressA;
	KeyArray[EKeyInfo::MovingOnGround] = GetCharacterMovement()->IsMovingOnGround();
	KeyArray[EKeyInfo::HoldingWeapon] = WeaponType == EWeaponType::GreatSwdArmed ? true : false;
	KeyArray[EKeyInfo::Unarmed] = WeaponType == EWeaponType::Unarmed ? true : false;
	KeyArray[EKeyInfo::LT_A] = PressLT && PressA;
	KeyArray[EKeyInfo::WASD] = PressWASD;
	KeyArray[EKeyInfo::HuntingEdgeHit] = AtkOncePerMonta;
	KeyArray[EKeyInfo::RT] = PressRT;
	KeyArray[EKeyInfo::Release_RT] = !PressRT;
	KeyArray[EKeyInfo::NearWall] = PlayerValutComponent->IsNearWall();
	KeyArray[EKeyInfo::NotNearWall] = !PlayerValutComponent->IsNearWall();
	KeyArray[EKeyInfo::Release_RB] = !PressRB;

	PressMap[EButtons::A] = PressA;
	PressMap[EButtons::B] = PressB;
	PressMap[EButtons::X] = PressX;
	PressMap[EButtons::Y] = PressY;
	PressMap[EButtons::LT] = PressLT;
	PressMap[EButtons::RB] = PressRB;
	PressMap[EButtons::RT] = PressRT;
	PressMap[EButtons::RStick] = PressWASD;
}

void AMH_PlayerCharacter::MakeFalling()
{
	UE_LOG(LogTemp, Log, TEXT("MakeFalling Called"));


	bool ShouldFall = (WeaponType == EWeaponType::Unarmed) 
					&& !(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying());
	
	if (ShouldFall)
	{
		if (PressRB)
		{
			PlayAnimMontage(FIND_MONTAGE("Character.Player.Montage.RunFall"));
		}
		else
		{
			PlayAnimMontage(FIND_MONTAGE("Character.Player.Montage.WalkFall"));
		}
	}


}

void AMH_PlayerCharacter::PlayValutMontage()
{
	bool bCanPlayMontage = (!GetCharacterMovement()->IsFalling() &&	WeaponType == EWeaponType::Unarmed &&
								LevelType == ELevelType::Field && !Busy);
		
	
	PlayerValutComponent->SetCanPlayValutMontage(bCanPlayMontage);

	
	//	switch (ValutMontage)
	//	{
	//	case EValutMontage::WallRun:
	//		if (PressRB && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	//		{
	//			PlayAnimMontage(VaultMontages[ValutMontage]);
	//		}
	//		break;
	//	case EValutMontage::JumpOver:
	//		Busy = true;
	//		PlayAnimMontage(VaultMontages[ValutMontage]);
	//		break;
	//	case EValutMontage::Vault:
	//		Busy = true;
	//		PlayAnimMontage(VaultMontages[ValutMontage]);
	//		break;
	//	default:
	//		break;
	//	}
	
}


// Called to bind functionality to input
void AMH_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMH_PlayerCharacter::ULook);	
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMH_PlayerCharacter::URotate);

	// Press Boolean On
	EnhancedInputComponent->BindAction(IA_A, ETriggerEvent::Started, this, &AMH_PlayerCharacter::ComboStartA);
	EnhancedInputComponent->BindAction(IA_Y, ETriggerEvent::Started, this, &AMH_PlayerCharacter::ComboStartY);
	EnhancedInputComponent->BindAction(IA_B, ETriggerEvent::Started, this, &AMH_PlayerCharacter::ComboStartB);
	EnhancedInputComponent->BindAction(IA_X, ETriggerEvent::Started, this, &AMH_PlayerCharacter::PressXOn);
	EnhancedInputComponent->BindAction(IA_RB, ETriggerEvent::Started, this, &AMH_PlayerCharacter::PressRBOn);
	EnhancedInputComponent->BindAction(IA_RT, ETriggerEvent::Started, this, &AMH_PlayerCharacter::ComboStartRT);
	EnhancedInputComponent->BindAction(IA_LT, ETriggerEvent::Started, this, &AMH_PlayerCharacter::PressLTOn);

	// Press Boolean Off
	EnhancedInputComponent->BindAction(IA_A, ETriggerEvent::Completed, this, &AMH_PlayerCharacter::PressAOff);
	EnhancedInputComponent->BindAction(IA_B, ETriggerEvent::Completed, this, &AMH_PlayerCharacter::PressBOff);
	EnhancedInputComponent->BindAction(IA_X, ETriggerEvent::Completed, this, &AMH_PlayerCharacter::PressXOff);
	EnhancedInputComponent->BindAction(IA_Y, ETriggerEvent::Completed, this, &AMH_PlayerCharacter::PressYOff);
	EnhancedInputComponent->BindAction(IA_RB, ETriggerEvent::Completed, this, &AMH_PlayerCharacter::PressRBOff);
	EnhancedInputComponent->BindAction(IA_RT, ETriggerEvent::Completed, this, &AMH_PlayerCharacter::PressRTOff);
	EnhancedInputComponent->BindAction(IA_LT, ETriggerEvent::Completed, this, &AMH_PlayerCharacter::PressLTOff);
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &AMH_PlayerCharacter::PressWASDOff);

}

float AMH_PlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageTakeType == EDamageTakeType::Evade)
	{
		return DamageAmount;
	}


	if (DamageTakeType == EDamageTakeType::Default)
	{
		const UMHBasicDamageType* CustomDamageType = Cast<UMHBasicDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
		if (CustomDamageType)
		{
			if (CustomDamageType->DamageTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("DamageType.Roar"))))
			{
				UE_LOG(LogTemp, Log, TEXT("RoarAttack!"));

				if (ComboStartMontage.Contains(TEXT("Roar")))
				{
					PlayAnimMontage(ComboStartMontage[TEXT("Roar")]);
				}
			}
		}
		else
		{
			if (ComboStartMontage.Contains(TEXT("Damaged")))
			{
				PlayAnimMontage(ComboStartMontage[TEXT("Damaged")]);
			}
			float SetHp = FMath::Clamp(StatComponent->GetCurrentHp() - DamageAmount, 0.0f, StatComponent->GetMaxHp() + StatComponent->GetMaxHpModifier());
			StatComponent->SetCurrentHp(SetHp);
		}
	}
	// DamageTakeType == Tackle, Gaurd
	else 
	{
		float SetHp = FMath::Clamp(StatComponent->GetCurrentHp() - (DamageAmount * 0.3f), 0.0f, StatComponent->GetMaxHp() + StatComponent->GetMaxHpModifier());
		StatComponent->SetCurrentHp(SetHp);

		//UNiagaraSystem* GaurdVFX = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/MarketContent/SlashHitAndStabHit/Particles/Niagara/NS_ky_stabHit08.NS_ky_stabHit08'"));
		//if (GaurdVFX)
		//{
		//	UNiagaraFunctionLibrary::SpawnSystemAttached(GaurdVFX, Gswd, TEXT("None"), FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
		//}
	}
	return DamageAmount;
}

void AMH_PlayerCharacter::StartHitStop(float Time)
{
	CustomTimeDilation = 0.05f;

	FTimerHandle TimeHandle;
	GetWorldTimerManager().SetTimer(TimeHandle, this, &AMH_PlayerCharacter::EndHitStop, 1.0f, false, Time);

}


void AMH_PlayerCharacter::CameraShake(bool IsStrong)
{
	if (!IsStrong)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CamShakeWeak, GetActorLocation(), 0.0f, 1000.0f);
	}
	else
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CamShakeStrong, GetActorLocation(), 0.0f, 1000.0f);
	}

}

void AMH_PlayerCharacter::TurnOnBuffEffect()
{
	if (!BuffEffect->IsActive())
	{
		BuffEffect->SetActive(true);
		StatComponent->SetAtkModifier((StatComponent->GetAtk() * 0.1f) + (StatComponent->GetAtkModifier()));
	}


}

void AMH_PlayerCharacter::ManualMoveBegin()
{
	ChargeStopped();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	ANSTimer = 0.0f;
}

void AMH_PlayerCharacter::ManualMoveTick(float ManualMoveSpeed, float ManualMoveZSpeed, float FlymodeTime, bool WallRun)
{
	ANSTimer += GetWorld()->GetDeltaSeconds();

	FVector DeltaMove = (GetActorUpVector() * ManualMoveZSpeed + GetActorRightVector() * ManualMoveSpeed) * GetWorld()->GetDeltaSeconds();

	// 지정된 상승 시간 이전
	if (ANSTimer < FlymodeTime)
	{
		AddActorWorldOffset(DeltaMove);

		// 벽에 닿았다면 착 달라붙기
		if (WallRun)
		{
			PlayerValutComponent->MakePlayerStickToWall();
		
		}
	}
	// 상승 시간 이후에는 자유 낙하 (무중력 상태 해제)
	else
	{
		if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}
		else
		{
			AddActorWorldOffset(DeltaMove);
		}
	}
}

void AMH_PlayerCharacter::ManualMoveEnd()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
}

void AMH_PlayerCharacter::ShootWirebugBegin(FName SocketName)
{
	WireBug->SetComponentTickEnabled(true);
	WireBug->SetVisibility(true, true);
	WireBug->SetWorldLocation(GetActorLocation());
	
	InitialZ = GetActorRotation().Yaw;
	InitialPosition = GetActorLocation();
}

void AMH_PlayerCharacter::ShootWirebugTick(float Distance, float ZDistance, float FDistance, float Movetime)
{
	ANSTimer += GetWorld()->GetDeltaSeconds();		
	float Ratio = FMath::Clamp((ANSTimer / Movetime), 0.0f, 1.0f);

	FVector BugPosition = InitialPosition + (GetActorRightVector() * Distance + GetActorUpVector() * ZDistance + GetActorForwardVector() * FDistance);
	WireBug->SetWorldLocation(FMath::Lerp(GetActorLocation(), BugPosition, Ratio));
}

void AMH_PlayerCharacter::ShootWirebugEnd()
{
	WireBug->SetVisibility(false, true);
	WireBug->SetComponentTickEnabled(false);
}

void AMH_PlayerCharacter::ComboTick(TMap<EKeyInfo, TObjectPtr<class UAnimMontage>> MontageMap, FName SectionName)
{
	UAnimMontage* ComboMontage = nullptr;

	if (MontageMap.IsEmpty())
	{
		return;
	}

	for (TPair<EKeyInfo, bool> key : KeyArray)
	{
		if (!key.Value)
		{
			continue;
		}

		if (MontageMap.Contains(key.Key))
		{
			ComboMontage = MontageMap[key.Key];
		}
	}


	if (KeyArray[EKeyInfo::Space] && WeaponType == EWeaponType::GreatSwdArmed)
	{
		PlayAnimMontage(RollMontages[KeyDirInt]);
		return;
	}
	
	if(ComboMontage == SpecialMoveMontages[0])
	{
		PlayAnimMontage(SpecialMoveMontages[KeyDirInt]);
		return;
	}


	PlayAnimMontage(ComboMontage);
	if (!SectionName.IsNone())
	{
		if (GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->IsValidSectionName(SectionName))
		{
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName);
		}		
	}
	
}

void AMH_PlayerCharacter::ComboEnd(bool IsChargeAtk)
{
	if (IsChargeAtk)
	{
		UMHGreatSwordComponent* GreatSwordComponent = FindComponentByClass<UMHGreatSwordComponent>();
		if (GreatSwordComponent)
		{
			GreatSwordComponent->StopCharge();
		}
	}

}

void AMH_PlayerCharacter::AttackBegin()
{
	AtkOncePerMonta = false;
}

void AMH_PlayerCharacter::AttackTick(FName AtkStartSocket, FName AtkEndSocket, float AtkRadius)
{
	if (AtkOncePerMonta)
	{
		return;
	}

	FVector TraceStartLocation = GetMesh()->GetSocketLocation(AtkStartSocket);
	FVector TraceEndLocation = GetMesh()->GetSocketLocation(AtkEndSocket);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
	ObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	ObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle));

	TArray<TObjectPtr<AActor>> IgnoreActors;
	FHitResult TraceResult;
	
	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),TraceStartLocation,TraceEndLocation,AtkRadius,ObjectTypeArray,false,IgnoreActors,EDrawDebugTrace::None,TraceResult,true);

	if (!TraceResult.bBlockingHit)
	{
		return;
	}

	AtkOncePerMonta = true;
	SwitchAtkMode();

	FName CurrentMontageName = FName(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->GetName());	
	const FMHPlayerAttackData* AtkData = GSwdAttackDataTableRef->FindRow<FMHPlayerAttackData>(CurrentMontageName, FString("FindRow"));
	
	if (!AtkData)
	{
		UE_LOG(LogTemp, Log, TEXT("AtkData Found Failed"));
		return;
	}

	bool IsCriticalHit = false;
	const float Damage = StatComponent->CaculateDamage(AtkData->DamageMul, IsCriticalHit);

	

	// Apply Damage
	UGameplayStatics::ApplyPointDamage(TraceResult.GetActor(),Damage,TraceResult.Location,TraceResult,GetController(),this, nullptr);

	// HitStop
	StartHitStop(AtkData->HitStopTime);

	// Camera Shake
	if (AtkData->CAMShakePower == 1)
	{
		CameraShake(false);
	}
	else if (AtkData->CAMShakePower == 2)
	{
		CameraShake(true);
	}

	// Hit VFX	
	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(	GetWorld(),	HitEffect,TraceResult.Location,GetActorForwardVector().ToOrientationRotator(),	FVector(1, 1, 1));
	}

	// Critical Hit VFX
	if (IsCriticalHit && CriticalHitEffect != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),CriticalHitEffect,TraceResult.Location,GetActorForwardVector().ToOrientationRotator(),FVector(1.5, 1.5, 0.1));
	}
}

void AMH_PlayerCharacter::DamageTakeBegin(EDamageTakeType DmgTakeType)
{
	DamageTakeType = DmgTakeType;
}

void AMH_PlayerCharacter::DamageTakeEnd()
{
	DamageTakeType = EDamageTakeType::Default;
}

void AMH_PlayerCharacter::ValutBegin(float Offset)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	const float Delta = PlayerValutComponent->GetWallDimensionZ() - GetActorLocation().Z - Offset;
	AddActorWorldOffset(FVector(0, 0, Delta));
}

void AMH_PlayerCharacter::ValutEnd()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	Busy = false;
}

void AMH_PlayerCharacter::SetupCharacterWidget(UMHUserWidget* InUserWidget)
{
	UE_LOG(LogTemp, Log, TEXT("SetupCharacterWidget Called"));
	UUtusiHPBarWidget* HpBarWidget = Cast<UUtusiHPBarWidget>(InUserWidget);	
	if (HpBarWidget)
	{		
		UE_LOG(LogTemp, Log, TEXT("HpBarWidget Cast Success"));
		HpBarWidget->SetMaxHp(StatComponent->GetMaxHp());
		HpBarWidget->UpdateHpBar(StatComponent->GetCurrentHp());

		StatComponent->OnCurrentHpChanged.AddUObject(HpBarWidget, &UUtusiHPBarWidget::UpdateHpBar);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("HpBarWidget Cast Failed"));
	}

}

void AMH_PlayerCharacter::SetPlayerState(EWeaponType Type)
{	
	WeaponType = Type;
}

void AMH_PlayerCharacter::AttachGreatSwdComponent()
{
	UMHGreatSwordComponent* GreatSwordComponent = NewObject<UMHGreatSwordComponent>(this);
	if (GreatSwordComponent)
	{
		// 컴포넌트를 소유한 액터에 추가
		GreatSwordComponent->RegisterComponent();						

		UE_LOG(LogTemp, Log, TEXT("Custom ActorComponent added dynamically!"));
	}
}



void AMH_PlayerCharacter::SwitchAtkMode()
{
}
