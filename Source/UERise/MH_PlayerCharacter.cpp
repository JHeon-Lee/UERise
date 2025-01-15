// Fill out your copyright notice in the Description page of Project Settings.

#include "MH_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CharacterStat/UtusiStatComponent.h"
#include "Animation/MHAnimInstancePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "GameData/MH_GlobalDamageType.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/DataTable.h"
#include "UI/UtusiHPBarWidget.h"
#include "UI/MHWidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/TimelineComponent.h"
#include "Character/Component/MH_PlayerStatComponent.h"
#include "GameData/MHPlayerAttackData.h"
#include "LegacyCameraShake.h"

//F:\UE_5.1\UE_5.1\Engine\Source\Runtime\Engine\Classes\Particles\ParticleSystemComponent.h

// Sets default values
AMH_PlayerCharacter::AMH_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ComponentAttach();
	InputSystemSetting();
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Script/Engine.DataTable'/Game/Characters/Utusi/Data/PlayerGSwdAttackDataTable.PlayerGSwdAttackDataTable'"));
	if (DataTable.Object != nullptr)
	{
		GSwdAttackDataTableRef = DataTable.Object;
	}

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

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Script/Engine.CurveFloat'/Game/Characters/Utusi/Data/OpacityFloat.OpacityFloat'"));
	if (Curve.Object)
	{
		OpacityCurve = Curve.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> OverlayMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/armorTest/BodyGlareMatInst.BodyGlareMatInst'"));
	if (OverlayMaterial.Object)
	{
		OverlayMaterialInstanceRef = OverlayMaterial.Object;
	}

}

void AMH_PlayerCharacter::ComponentAttach()
{
	//Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 450.0f;
	CameraBoom->TargetOffset = FVector(0, 0, 70);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Mesh Attach
	WireBug = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WireBug"));
	WireBug->SetupAttachment(GetMesh());
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WireBugMeshRef(TEXT("/ Script / Engine.SkeletalMesh'/Game/Characters/WireBug/ec008_00_meshout.ec008_00_meshout'"));
	if (WireBugMeshRef.Object)
	{
		WireBug->SetSkeletalMesh(WireBugMeshRef.Object);
	}

	Gswd = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gswd"));
	Gswd->SetupAttachment(GetMesh(), TEXT("B_Weapon"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GswdMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/armorTest/Wyvern/G_Swd086_meshout.G_Swd086_meshout'"));
	if (GswdMeshRef.Object)
	{
		Gswd->SetSkeletalMesh(GswdMeshRef.Object);
	}

	BodyFlameEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BodyFlameEffect"));
	BodyFlameEffect->SetupAttachment(GetMesh());

	BuffEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BuffEffect"));
	BodyFlameEffect->SetupAttachment(GetMesh(), TEXT("R_Grip_00"));

	BugEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BugEffect"));
	BugEffect->SetupAttachment(WireBug);

	SwdFlameEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwdFlameEffect"));
	SwdFlameEffect->SetupAttachment(Gswd);
}

void AMH_PlayerCharacter::InputSystemSetting()
{

	//Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/ Script / EnhancedInput.InputMappingContext'/Game/InputSystem/UtusiMappingContext.UtusiMappingContext'"));
	if (InputMappingContextRef.Object)
	{
		UtusiMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionA(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/IA_A.IA_A'"));

	if (nullptr != InputActionA.Object)
	{
		IA_A = InputActionA.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionB(TEXT("/ Script / EnhancedInput.InputAction'/Game/InputSystem/IA_B.IA_B'"));
	if (nullptr != InputActionB.Object)
	{
		IA_B = InputActionB.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionX(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/IA_X.IA_X'"));
	if (nullptr != InputActionX.Object)
	{
		IA_X = InputActionX.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionY(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/IA_Y.IA_Y'"));
	if (nullptr != InputActionY.Object)
	{
		IA_Y = InputActionY.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRB(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/IA_RB.IA_RB'"));
	if (nullptr != InputActionRB.Object)
	{
		IA_RB = InputActionRB.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRT(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/IA_RT.IA_RT'"));
	if (nullptr != InputActionRT.Object)
	{
		IA_RT = InputActionRT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLT(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/IA_LT.IA_LT'"));
	if (nullptr != InputActionLT.Object)
	{
		IA_LT = InputActionLT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		IA_Move = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		IA_Look = InputActionLookRef.Object;
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

	if (OverlayMaterialInstanceRef)
	{
		OverlayMaterialDynamicInstance =UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), OverlayMaterialInstanceRef);
		Gswd->SetOverlayMaterial(OverlayMaterialDynamicInstance);
	}

	FOnTimelineFloat GSwdOverlayMaterialOpacity;
	GSwdOverlayMaterialOpacity.BindUFunction(this, FName("OpacityUpdate"));
	OpacityFloatTimeline.AddInterpFloat(OpacityCurve, GSwdOverlayMaterialOpacity);

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
}

void AMH_PlayerCharacter::ULook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	CameraBoom->AddRelativeRotation(FRotator(-1 * LookAxisVector.Y, LookAxisVector.X, 0));
	FRotator BoomRotate = CameraBoom->GetRelativeRotation();	
	if (BoomRotate.Pitch < -70)
		BoomRotate.Pitch = -70;
	if (BoomRotate.Pitch > 70)
		BoomRotate.Pitch = 70;
	CameraBoom->SetRelativeRotation(BoomRotate);
}

void AMH_PlayerCharacter::ComboStartA()
{	
	PressA = true;

	// Play Montage When Moving On Ground and not any Montage Playing
	if (GetCharacterMovement()->IsMovingOnGround() && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		if (WeaponType == EWeaponType::Unarmed)
		{
			PlayAnimMontage(ComboStartMontage[TEXT("AUnarmed")]);
		}
		else if (WeaponType == EWeaponType::Armed)
		{
			PlayAnimMontage(ComboStartMontage[TEXT("AArmed")]);
		}
	}			
}

void AMH_PlayerCharacter::ComboStartY()
{
	PressYOn();

	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}

	if (WeaponType == EWeaponType::Armed)
	{
		if (PressB)
		{
			PlayAnimMontage(ComboStartMontage[TEXT("YBArmed")]);
		}
		else if (PressLT && CurWireBugStack > 0)
		{
			PlayAnimMontage(ComboStartMontage[TEXT("YLTArmed")]);
		}
		else if(!PressLT)
		{
			PlayAnimMontage(ComboStartMontage[TEXT("YArmed")]);
		}
	}

	else if (WeaponType == EWeaponType::Unarmed)
	{
		if (!PressLT && PressWASD)
		{
			PlayAnimMontage(ComboStartMontage[TEXT("YWASDUnarmed")]);
		}
		else if (PressLT && CurWireBugStack > 0)
		{
			PlayAnimMontage(ComboStartMontage[TEXT("YLTUnarmed")]);
		}
	}
}

void AMH_PlayerCharacter::ComboStartRT()
{
	PressRTOn();

	if (!(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()))
	{
		PlayAnimMontage(ComboStartMontage[TEXT("RT")]);
	}
}

void AMH_PlayerCharacter::ComboStartB()
{
	PressBOn();

	if (!(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) && WeaponType == EWeaponType::Armed)
	{
		PlayAnimMontage(ComboStartMontage[TEXT("BArmed")]);
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
	ValutCheck();
	MakeFalling();

	OpacityFloatTimeline.TickTimeline(DeltaTime);
}

void AMH_PlayerCharacter::KeyPressCheck()
{
	// KeyArray Set

	while (KeyArray.Num() < 20)
	{
		KeyArray.Push(false);
	}

	KeyArray[0] = true;
	KeyArray[1] = PressLT && PressY && (CurWireBugStack > 0 ? true : false);
	KeyArray[2] = PressLT && PressB && (CurWireBugStack > 0 ? true : false);
	KeyArray[3] = PressY && PressB;
	KeyArray[4] = PressY && PressWASD;
	KeyArray[5] = PressY;
	KeyArray[6] = !PressY;
	KeyArray[7] = PressB;
	KeyArray[8] = !PressB;
	KeyArray[9] = PressA;	
	KeyArray[10] = GetCharacterMovement()->IsMovingOnGround();
	KeyArray[11] = WeaponType == EWeaponType::Armed ? true : false;
	KeyArray[12] = WeaponType == EWeaponType::Unarmed ? true : false;
	KeyArray[13] = PressLT && PressA;
	KeyArray[14] = PressWASD;
	KeyArray[15] = AtkOncePerMonta;
	KeyArray[16] = PressRT;
	KeyArray[17] = !PressRT;	
	//18, 19 는 Valut Check 중에
	KeyArray[20] = !PressRB;


	// KeyDirInt Set
	KeyDirInt = 0;

	if(PressWASD)
	{
		if		(KeyDir > -45.0f && KeyDir <= 45.0f)	KeyDirInt = 0;
		else if (KeyDir > -135.0f && KeyDir <= -45.0f)	KeyDirInt = 3;
		else if (KeyDir > 45.0f && KeyDir <= 135.0f)	KeyDirInt = 1;
		else											KeyDirInt = 2;	
	}
}

void AMH_PlayerCharacter::MakeFalling()
{
	bool ShouldFall =    (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) 
					  && (!GetCharacterMovement()->IsMovingOnGround()) 
				      && (WeaponType == EWeaponType::Unarmed);
	
	if (!ShouldFall)
	{
		return;
	}

	if (PressRB)
	{
		if (ComboStartMontage.Contains(TEXT("RunFall")))
		{
			PlayAnimMontage(ComboStartMontage[TEXT("RunFall")]);
		}
	}
	else
	{
		if (ComboStartMontage.Contains(TEXT("WalkFall")))
		{
			PlayAnimMontage(ComboStartMontage[TEXT("WalkFall")]);
		}
	}

}

void AMH_PlayerCharacter::ValutCheck()
{
	GetFrontObjectLocation();

	bool CanPlayVaultMontage =
		(
			DistanceToWall > 0.0f &&
			DistanceToWall <= 100.0f &&
			Busy == false &&
			!GetCharacterMovement()->IsFalling() &&
			WeaponType == EWeaponType::Unarmed&&
			LevelType == ELevelType::Field
		);

	if (CanPlayVaultMontage)
	{
		PlayValutMontatge();
	}
}

void AMH_PlayerCharacter::GetFrontObjectLocation()
{
	FVector TraceStart = GetActorLocation() - FVector(0, 0, 30);
	FVector TraceEnd = TraceStart + GetActorRightVector() * 100;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
	ObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<TObjectPtr<AActor>> IgnoreActors;
	FHitResult TraceResult;

	UKismetSystemLibrary::SphereTraceSingleForObjects	(
		GetWorld(), 
		TraceStart, 
		TraceEnd, 
		10.0f, 
		ObjectTypeArray, 
		false, 
		IgnoreActors, 
		EDrawDebugTrace::None, 
		TraceResult, 
		true
	);
	
	KeyArray[18] = TraceResult.bBlockingHit;
	KeyArray[19] = !TraceResult.bBlockingHit;

	if (TraceResult.bBlockingHit)
	{
		ImpactNormal		= TraceResult.ImpactNormal;
		WallRotZ			= TraceResult.ImpactNormal.ToOrientationRotator().Yaw +90.0f;
		WallSlope			= UKismetMathLibrary::DegAcos(FVector::DotProduct(TraceResult.ImpactNormal, GetActorUpVector()));
		InitialImpactPoint  = TraceResult.ImpactPoint;
		DistanceToWall		= FVector::Distance(GetActorLocation(), InitialImpactPoint);
		GetObjectDimension();
	}
	else
	{
		InitialImpactPoint = FVector(0, 0, 0);
		DistanceToWall = 0.0;
		CanWallRun = false;
	}
}

void AMH_PlayerCharacter::GetObjectDimension()
{
	const uint8 LastIndex = 30;
	uint8 CurIndex = 0;

	for (; CurIndex <= LastIndex; CurIndex++)
	{
		FVector TraceStart =  InitialImpactPoint;
		TraceStart.Z += CurIndex * 15;

		FVector TraceEnd = TraceStart + GetActorRightVector() * 150;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
		ObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		TArray<TObjectPtr<AActor>> IgnoreActors;
		FHitResult TraceResult;

		UKismetSystemLibrary::LineTraceSingleForObjects	(
			GetWorld(),
			TraceStart,
			TraceEnd,
			ObjectTypeArray,
			false,
			IgnoreActors,
			EDrawDebugTrace::None,
			TraceResult,
			true
		);

		if (TraceResult.bBlockingHit)
		{
			ImpactPoint = TraceResult.ImpactPoint;
		}
		else
		{
			break;
		}
	}
}

void AMH_PlayerCharacter::PlayValutMontatge()
{
	float DistanceToVaultPoint = ImpactPoint.Z - GetActorLocation().Z;

	if (DistanceToVaultPoint > 150.0f && PressRB && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		PlayAnimMontage(VaultMontages[EValutMontage::WallRun]);
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DistanceToVaultPoint, 40.0f, 150.0f, false))
	{
		Busy = true;
		VaultOffset = 20.0f;
		PlayAnimMontage(VaultMontages[EValutMontage::JumpOver]);
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DistanceToVaultPoint, -40.0f, 40.0f))
	{
		Busy = true;
		VaultOffset = -45.0f;
		PlayAnimMontage(VaultMontages[EValutMontage::Vault]);
	}
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

	float SetHp = FMath::Clamp(StatComponent->GetCurrentHp() - DamageAmount, 0.0f, StatComponent->GetMaxHp());
	StatComponent->SetCurrentHp(SetHp);

	return DamageAmount;
}

void AMH_PlayerCharacter::StartHitStop(float Time)
{
	CustomTimeDilation = 0.05f;

	FTimerHandle TimeHandle;
	GetWorldTimerManager().SetTimer(TimeHandle, this, &AMH_PlayerCharacter::EndHitStop, 1.0f, false, Time);

}

void AMH_PlayerCharacter::StopCharge()
{
	FTimerHandle TimeHandle;
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AMH_PlayerCharacter::StopChargeCallback, 2.0, false);

}

void AMH_PlayerCharacter::StopChargeCallback()
{
	SwdFlameEffect->SetActive(false);
	BodyFlameEffect->SetActive(false);
	OverlayMaterialDynamicInstance->SetScalarParameterValue(TEXT("Opacity"), 0.0);
	ChargeStep = 0;
}

void AMH_PlayerCharacter::SwdAttachToSocket(FName socketName)
{
	bool isValidSocket = GetMesh()->DoesSocketExist(socketName);
	if (isValidSocket)
	{
		Gswd->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), socketName);
		if (socketName == TEXT("B_Weapon"))
		{
			WeaponType = EWeaponType::Unarmed;
		}
		else if (socketName == TEXT("L_Weapon"))
		{
			WeaponType = EWeaponType::Armed;
		}
	}	
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
void AMH_PlayerCharacter::GSwdFstCharge()
{
	OpacityFloatTimeline.PlayFromStart();
	OverlayMaterialDynamicInstance->SetVectorParameterValue(TEXT("Color"), FVector4(50.0, 8.0, 8.0));

	ChargeStep = 1;
}

void AMH_PlayerCharacter::GSwdSndCharge()
{
	OpacityFloatTimeline.PlayFromStart();
	OverlayMaterialDynamicInstance->SetVectorParameterValue(TEXT("Color"), FVector4(50.0, 8.0, 1.3));
	ChargeStep = 2;
}

void AMH_PlayerCharacter::GSwdTrdCharge()
{
	OpacityFloatTimeline.PlayFromStart();
	OverlayMaterialDynamicInstance->SetVectorParameterValue(TEXT("Color"), FVector4(19.5, 50.0, 50.0));

	BodyFlameEffect->SetActive(true);
	SwdFlameEffect->SetActive(true);

	// ChargeEndVFX
	UNiagaraSystem* ChargeEndVFX = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/MarketContent/SlashHitAndStabHit/Particles/Niagara/NS_ky_stabHit08.NS_ky_stabHit08'"));
	if (ChargeEndVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(ChargeEndVFX, Gswd, TEXT("None"), FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
	}

	ChargeStep = 3;
}

void AMH_PlayerCharacter::TurnOnBuffEffect()
{
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
		if (WallRun && !InitialImpactPoint.IsZero())
		{
			FVector HitPoint = (GetActorRightVector() * -50) + InitialImpactPoint;
			HitPoint.Z = GetActorLocation().Z;
			SetActorLocation(HitPoint);

			FVector RotVector = FVector(ImpactNormal.X, ImpactNormal.Y, 0.0);			
			FRotator Rot = RotVector.ToOrientationRotator();
			Rot.Yaw += 90.0f;
			SetActorRotation(Rot);			
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
	uint8 ArrayIndex = 0;
	UAnimMontage* ComboMontage = nullptr;

	if (MontageMap.IsEmpty())
	{
		return;
	}

	for (; ArrayIndex < KeyArray.Num(); ArrayIndex++)
	{
		if (!KeyArray[ArrayIndex])
		{
			continue;
		}

		if (MontageMap.Contains((EKeyInfo)ArrayIndex))
		{
			ComboMontage = MontageMap[(EKeyInfo)ArrayIndex];
			break;
		}
	}


	if (ArrayIndex == (uint8)EKeyInfo::Space && WeaponType == EWeaponType::Armed)
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
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName);
	}
	
}

void AMH_PlayerCharacter::ComboEnd(bool IsChargeAtk)
{
	if (IsChargeAtk)
	{
		StopCharge();
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

void AMH_PlayerCharacter::OpacityUpdate(float Opcity)
{
	OverlayMaterialDynamicInstance->SetScalarParameterValue(TEXT("Opacity"), Opcity);
}

void AMH_PlayerCharacter::SwitchAtkMode()
{
}
