// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MHRathalosSetterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMHRathalosSetterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UERISE_API IMHRathalosSetterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetMovementModeWalking() = 0;
	virtual void SetNoCollision() = 0;
};
