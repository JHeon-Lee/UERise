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
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"
#include "UI/UtusiHPBarWidget.h"
#include "UI/MHWidgetComponent.h"

// Sets default values
AMH_PlayerCharacter::AMH_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ComponentAttach();
	InputSystemSetting();
	
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
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(UtusiMappingContext, 0);
	}

	if (OverlayMaterialRef)
	{
		Gswd->SetOverlayMaterial(OverlayMaterialRef);
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
	//15
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
		WallRotZ			= TraceResult.ImpactNormal.ToOrientationRotator().Yaw + 90.0f;
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
			RotVector = FRotator(0.0, 0.0, 90.0).RotateVector(RotVector);
			SetActorRotation(RotVector.ToOrientationRotator());			
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

void AMH_PlayerCharacter::ComboTick(TMap<EKeyInfo, TObjectPtr<class UAnimMontage>> MontageMap, bool IsChargeAtk, FName SectionName)
{
	uint8 ArrayIndex = 0;

	for (; KeyArray.Num(); ArrayIndex++)
	{
		if (KeyArray[ArrayIndex])
		{
			UAnimMontage* ComboMontage = MontageMap.Find((EKeyInfo)ArrayIndex)->Get(); 
			if (IsValid(ComboMontage))
			{

			}

		}
	}
}

void AMH_PlayerCharacter::AttackBegin()
{
}

void AMH_PlayerCharacter::AttackTick()
{
}

void AMH_PlayerCharacter::AttackEnd()
{
}
