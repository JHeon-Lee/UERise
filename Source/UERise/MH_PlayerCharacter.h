// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/MH_AnimNotifyInterface.h"
#include "MH_PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	Armed UMETA(DisplayName = "Armed"),
	Riding UMETA(DisplayName = "Riding"),
	DragonRide UMETA(DisplayName = "DragonRide")
};

UCLASS()
class UERISE_API AMH_PlayerCharacter : public ACharacter, public IMH_AnimNotifyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMH_PlayerCharacter();

	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float TargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float DistanceToWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float VaultOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float WallSlope;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float WallRotZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector ImpactPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector InitialImpactPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector WarpPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector ImpactNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressWASD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressRB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressRT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressLT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	int32 KeyDirInt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	float KeyDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	TArray<bool> KeyArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool AtkOncePerMonta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool Busy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool ToOppositeDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool CanWallRun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool ManualMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool WallRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool NeedZVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float ManualMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float ManualMoveZSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float ANSTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float InitialZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentTrgtZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 ChargeStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FVector2D ActionValue;

	//----------------------------------Stat---------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHpModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStaminaModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxWeaponDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentWeaponDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AtkModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Critical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 WireBugStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	EWeaponType weaponType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> WireBug;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> UtusiMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_A;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_B;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_X;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Y;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Look;	

public:
	void URotate(const FInputActionValue& Value);
	void ULook(const FInputActionValue& Value);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//Notify Section
	virtual void NotifyBegin() override;
	virtual void NotifyTick() override;
	virtual void NotifyEnd() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChargeStopped();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ANS_Attack();

};
