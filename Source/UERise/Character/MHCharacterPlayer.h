// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/MHCharacterGetterInterface.h"
#include "Interface/MHAnimationSocketInterface.h"
#include "MHCharacterPlayer.generated.h"

UENUM()
enum class ECharacterCameraType : uint8
{
	ThirdPersonCombatView,
	ThirdPersonTownView,
	FirstPersonView
};

UCLASS()
class UERISE_API AMHCharacterPlayer : public ACharacter, public IMHCharacterGetterInterface, public IMHAnimationSocketInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMHCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Mesh Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> HelmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> ArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> WaistMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> LegMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> WeaponMesh;

	// Character Camera Mode Section
protected:
	void ChangeCharacterCamera();
	void SetCharacterCamera(ECharacterCameraType NewCharacterCameraType);
	void SetCharacterCameraData(const class UMHCharacterCameraData* CharacterCameraData);

	UPROPERTY(EditAnywhere, Category = CharacterCamera, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterCameraType, class UMHCharacterCameraData*> CharacterCameraManager;
	
	// Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> PlayerSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	ECharacterCameraType CurrentCharacterCameraType;

	// Input Section
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThirdPersonMoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThirdPersonLookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LMBAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SpaceAction;

	// 디버그용, 후에 삭제
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TmpChangeViewAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<class UMHWidgetComponent> PlayerWidgetComponent;

	void ThirdPersonMove(const FInputActionValue& Value);
	void ThirdPersonLook(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Character)
	FVector2D MovementVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Character)
	FVector CurFrontVector;

	// Interface override Section
	virtual uint8 GetIsDrawing() override;
	virtual void ChangeWeaponLocation() override;

	// Status Section
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Character)
	uint8 bIsDrawing : 1;
};
