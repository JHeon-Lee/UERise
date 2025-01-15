// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MHCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MHCharacterCameraData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "UI/UtusiHPBarWidget.h"
#include "UI/MHWidgetComponent.h"
#include "Physics/MHCollision.h"

// Sets default values
AMHCharacterPlayer::AMHCharacterPlayer()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_MHCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -99.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ImportSource/CharacterSK/bone.bone"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/ABP_MHCharacterPlayer.ABP_MHCharacterPlayer_C'"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	HelmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmMesh"));
	HelmMesh->SetupAttachment(GetMesh());
	HelmMesh->SetLeaderPoseComponent(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HelmMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ImportSource/CharacterEquipment/helm604.helm604'"));
	if (HelmMeshRef.Object)
	{
		HelmMesh->SetSkeletalMesh(HelmMeshRef.Object);
	}

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetMesh());
	BodyMesh->SetLeaderPoseComponent(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ImportSource/CharacterEquipment/body604.body604'"));
	if (BodyMeshRef.Object)
	{
		BodyMesh->SetSkeletalMesh(BodyMeshRef.Object);
	}

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->SetupAttachment(GetMesh());
	ArmMesh->SetLeaderPoseComponent(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ArmMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ImportSource/CharacterEquipment/arm604.arm604'"));
	if (ArmMeshRef.Object)
	{
		ArmMesh->SetSkeletalMesh(ArmMeshRef.Object);
	}

	WaistMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WaistMesh"));
	WaistMesh->SetupAttachment(GetMesh());
	WaistMesh->SetLeaderPoseComponent(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WaistMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ImportSource/CharacterEquipment/wst604.wst604'"));
	if (WaistMeshRef.Object)
	{
		WaistMesh->SetSkeletalMesh(WaistMeshRef.Object);
	}

	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	LegMesh->SetupAttachment(GetMesh());
	LegMesh->SetLeaderPoseComponent(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> LegMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ImportSource/CharacterEquipment/leg604.leg604'"));
	if (LegMeshRef.Object)
	{
		LegMesh->SetSkeletalMesh(LegMeshRef.Object);
	}

	FName WeaponSocket(TEXT("Socket0"));
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ImportSource/CharacterEquipment/GSwd.GSwd'"));
	if (WeaponMeshRef.Object)
	{
		WeaponMesh->SetStaticMesh(WeaponMeshRef.Object);
	}

	// Camera
	PlayerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	PlayerSpringArm->SetupAttachment(RootComponent);
	PlayerSpringArm->TargetArmLength = 400.0f;
	PlayerSpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(PlayerSpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UMHCharacterCameraData> ThirdPersonCombatDataRef(TEXT("/Script/MonsterHunter.MHCharacterCameraData'/Game/CharacterCamera/DA_MHThirdPersonCombatView.DA_MHThirdPersonCombatView'"));
	if (ThirdPersonCombatDataRef.Object)
	{
		CharacterCameraManager.Add(ECharacterCameraType::ThirdPersonCombatView, ThirdPersonCombatDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMHCharacterCameraData> ThirdPersonTownDataRef(TEXT("/Script/MonsterHunter.MHCharacterCameraData'/Game/CharacterCamera/DA_MHThirdPersonTownView.DA_MHThirdPersonTownView'"));
	if (ThirdPersonTownDataRef.Object)
	{
		CharacterCameraManager.Add(ECharacterCameraType::ThirdPersonTownView, ThirdPersonTownDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMHCharacterCameraData> QuaterDataRef(TEXT("/Script/MonsterHunter.MHCharacterCameraData'/Game/CharacterCamera/DA_MHFirstPersonView.DA_MHFirstPersonView'"));
	if (QuaterDataRef.Object)
	{
		CharacterCameraManager.Add(ECharacterCameraType::FirstPersonView, QuaterDataRef.Object);
	}

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionThirdPersonMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ThirdPersonMove.IA_ThirdPersonMove'"));
	if (nullptr != InputActionThirdPersonMoveRef.Object)
	{
		ThirdPersonMoveAction = InputActionThirdPersonMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionThirdPersonLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_ThirdPersonLook.IA_ThirdPersonLook'"));
	if (nullptr != InputActionThirdPersonLookRef.Object)
	{
		ThirdPersonLookAction = InputActionThirdPersonLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeViewRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_TmpChangeView.IA_TmpChangeView'"));
	if (nullptr != InputActionChangeViewRef.Object)
	{
		TmpChangeViewAction = InputActionChangeViewRef.Object;
	}

	CurrentCharacterCameraType = ECharacterCameraType::ThirdPersonCombatView;

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

	// Status
	bIsDrawing = false;
}

void AMHCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterCamera(CurrentCharacterCameraType);
}

void AMHCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(ThirdPersonMoveAction, ETriggerEvent::Triggered, this, &AMHCharacterPlayer::ThirdPersonMove);
	EnhancedInputComponent->BindAction(ThirdPersonLookAction, ETriggerEvent::Triggered, this, &AMHCharacterPlayer::ThirdPersonLook);
	EnhancedInputComponent->BindAction(TmpChangeViewAction, ETriggerEvent::Triggered, this, &AMHCharacterPlayer::ChangeCharacterCamera);
}

void AMHCharacterPlayer::ChangeCharacterCamera()
{
	if (CurrentCharacterCameraType == ECharacterCameraType::FirstPersonView)
	{
		SetCharacterCamera(ECharacterCameraType::ThirdPersonCombatView);
	}
	else if (CurrentCharacterCameraType == ECharacterCameraType::ThirdPersonCombatView)
	{
		SetCharacterCamera(ECharacterCameraType::FirstPersonView);
	}
}

void AMHCharacterPlayer::SetCharacterCamera(ECharacterCameraType NewCharacterCameraType)
{
	UMHCharacterCameraData* NewCharacterCamera = CharacterCameraManager[NewCharacterCameraType];
	check(NewCharacterCamera);

	SetCharacterCameraData(NewCharacterCamera);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterCamera->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterCameraType = NewCharacterCameraType;
}

void AMHCharacterPlayer::SetCharacterCameraData(const UMHCharacterCameraData* CharacterCameraData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterCameraData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterCameraData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterCameraData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterCameraData->RotationRate;

	// Camera
	PlayerSpringArm->TargetArmLength = CharacterCameraData->TargetArmLength;
	PlayerSpringArm->SetRelativeRotation(CharacterCameraData->RelativeRotation);
	PlayerSpringArm->bUsePawnControlRotation = CharacterCameraData->bUsePawnControlRotation;
	PlayerSpringArm->bInheritPitch = CharacterCameraData->bInheritPitch;
	PlayerSpringArm->bInheritYaw = CharacterCameraData->bInheritYaw;
	PlayerSpringArm->bInheritRoll = CharacterCameraData->bInheritRoll;
	PlayerSpringArm->bDoCollisionTest = CharacterCameraData->bDoCollisionTest;
}

void AMHCharacterPlayer::ThirdPersonMove(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();

	CurFrontVector = GetActorForwardVector();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AMHCharacterPlayer::ThirdPersonLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

uint8 AMHCharacterPlayer::GetIsDrawing()
{
	return bIsDrawing;
}

void AMHCharacterPlayer::ChangeWeaponLocation()
{
	FName WeaponSocket0(TEXT("Socket0"));
	FName WeaponSocket1(TEXT("Socket1"));

	if (WeaponMesh->GetAttachSocketName() == WeaponSocket0)
	{
		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket1);
	}
	else if (WeaponMesh->GetAttachSocketName() == WeaponSocket1)
	{
		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket0);
	}
}


