// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MH_PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "MHAnimInstancePlayer.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHAnimInstancePlayer : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMHAnimInstancePlayer();



protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class AMH_PlayerCharacter> A_Utusi;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bOnGround;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bPressWASD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bPressRB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bPressA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bPressY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bPressX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	EWeaponType weaponType;

	



	// CharacterPlayer처럼 BP의 변수를 직접 가져오는 경우 인터페이스 사용해서 가져와야함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDrawing : 1;
};
