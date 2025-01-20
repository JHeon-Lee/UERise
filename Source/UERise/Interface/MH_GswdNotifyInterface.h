// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MH_GswdNotifyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMH_GswdNotifyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UERISE_API IMH_GswdNotifyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SwdAttachToSocket(FName socketName) = 0;

	virtual void GSwdFstCharge() = 0;
	virtual void GSwdSndCharge() = 0;
	virtual void GSwdTrdCharge() = 0;

	virtual void TurnOnBuffEffect() = 0;


};
