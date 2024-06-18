// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MHAnimInstanceRathalos.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHAnimInstanceRathalos : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMHAnimInstanceRathalos();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsDetecting : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsFlying : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsDragonRiding : 1;
};
