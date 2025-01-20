// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MHCharacterMovementComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMovementFallingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMovementWalkingDelegate);

UCLASS()
class UERISE_API UMHCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	

protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

public:
	FOnMovementFallingDelegate OnMovementFallingDelegate;
	FOnMovementWalkingDelegate OnMovementWalkingDelegate;



	EMovementMode PrevMovementMode;
};
