// Fill out your copyright notice in the Description page of Project Settings.

#include "MH_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CharacterStat/UtusiStatComponent.h"
#include "UI/UtusiHPBarWidget.h"
#include "UI/MHWidgetComponent.h"

// Sets default values
AMH_PlayerCharacter::AMH_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 450.0f;
	CameraBoom->TargetOffset = FVector(0, 0, 70);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	WireBug = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WireBug"));
	WireBug->SetupAttachment(GetMesh());
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WireBugMeshRef(TEXT("/ Script / Engine.SkeletalMesh'/Game/Characters/WireBug/ec008_00_meshout.ec008_00_meshout'"));
	
	AGRInventoryComponent = CreateDefaultSubobject<UAGR_InventoryManager>("AGRInventory");



	if (WireBugMeshRef.Object)
	{
		WireBug->SetSkeletalMesh(WireBugMeshRef.Object);
	}


	//Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/ Script / EnhancedInput.InputMappingContext'/Game/InputSystem/UtusiMappingContext.UtusiMappingContext'"));
	if(InputMappingContextRef.Object)
	{
		UtusiMappingContext = InputMappingContextRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionA(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/IA_A.IA_A'"));
	if (nullptr != InputActionA.Object)
	{
		IA_A = InputActionA.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionB(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/IA_B.IA_B'"));
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
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
}

void AMH_PlayerCharacter::URotate(const FInputActionValue& Value)
{
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

// Called every frame
void AMH_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMH_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMH_PlayerCharacter::ULook);
}

void AMH_PlayerCharacter::NotifyBegin()
{

}

void AMH_PlayerCharacter::NotifyTick()
{

}

void AMH_PlayerCharacter::NotifyEnd()
{

}


