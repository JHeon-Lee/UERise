// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MHMonsterAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMHMonsterAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UERISE_API IMHMonsterAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool AttackHitCheck1() = 0; // Tail
	virtual bool AttackHitCheck2() = 0; // Jump
	virtual bool AttackHitCheck3() = 0; // Head
	virtual bool AttackHitCheck4() = 0; // Rush
};
