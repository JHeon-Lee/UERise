// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData/MHGlobalEnum.h"
#include "MH_WeaponComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMH_WeaponComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UERISE_API IMH_WeaponComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetPlayerState(EWeaponType Type) = 0;

};
