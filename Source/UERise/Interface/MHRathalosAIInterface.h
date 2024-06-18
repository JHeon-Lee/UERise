// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MHRathalosAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMHRathalosAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAIRathalosAttackFinished);
DECLARE_DELEGATE(FAIRathalosRotateFinished);
DECLARE_DELEGATE(FAIRathalosRotate2Finished);
DECLARE_DELEGATE(FAIRathalosMoveFinished);
DECLARE_DELEGATE(FAIRathalosRoarFinished);
DECLARE_DELEGATE(FAIRathalosFlyFinished);
DECLARE_DELEGATE(FAIRathalosFlyRotateFinished);
DECLARE_DELEGATE(FAIRathalosEscapeRotateFinished);
DECLARE_DELEGATE(FAIRathalosEscapeMoveFinished);

/**
 * 
 */
class UERISE_API IMHRathalosAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackShortRange() = 0;
	virtual float GetAIAttackLongRange() = 0;
	virtual float GetAILostTargetDistance() = 0;

	virtual void SetAIAttackDelegate(const FAIRathalosAttackFinished& InOnAttackFinished) = 0;
	virtual void SetAIRotateDelegate(const FAIRathalosRotateFinished& InOnRotateFinished) = 0;
	virtual void SetAIRotate2Delegate(const FAIRathalosRotate2Finished& InOnRotate2Finished) = 0;
	virtual void SetAIMoveDelegate(const FAIRathalosMoveFinished& InOnMoveFinished) = 0;
	virtual void SetAIRoarDelegate(const FAIRathalosRoarFinished& InOnRoarFinished) = 0;
	virtual void SetAIFlyDelegate(const FAIRathalosFlyFinished& InOnFlyFinished) = 0;
	virtual void SetAIFlyRotateDelegate(const FAIRathalosFlyRotateFinished& InOnFlyRotateFinished) = 0;
	virtual void SetAIEscapeRotateDelegate(const FAIRathalosEscapeRotateFinished& InOnEscapeRotateFinished) = 0;
	virtual void SetAIEscapeMoveDelegate(const FAIRathalosEscapeMoveFinished& InOnEscapeMoveFinished) = 0;
	
	virtual void AttackPattern1AI() = 0;
	virtual void AttackPattern2AI() = 0;
	virtual void AttackPattern3AI() = 0;
	virtual void AttackPattern4AI() = 0;
	virtual void AttackPattern5AI() = 0;
	virtual void AttackPattern5_1AI() = 0;
	virtual void AttackPattern5_2AI() = 0;
	virtual void AttackPattern6AI() = 0;
	virtual void AttackPattern7AI() = 0;
	virtual void AttackPattern8AI() = 0;
	virtual void AttackPattern8_1AI() = 0;
	virtual void AttackPattern9AI() = 0;

	virtual void FlyAI() = 0;
	virtual void FlyRotateLeft90AI(float InAngleRate) = 0;
	virtual void FlyRotateLeft180AI(float InAngleRate) = 0;
	virtual void FlyRotateRight90AI(float InAngleRate) = 0;
	virtual void FlyRotateRight180AI(float InAngleRate) = 0;

	virtual void RotateLeft90AI(float InAngleRate) = 0;
	virtual void RotateLeft180AI(float InAngleRate) = 0;
	virtual void RotateRight90AI(float InAngleRate) = 0;
	virtual void RotateRight180AI(float InAngleRate) = 0;

	virtual void Rotate2Left90AI(float InAngleRate) = 0;
	virtual void Rotate2Left180AI(float InAngleRate) = 0;
	virtual void Rotate2Right90AI(float InAngleRate) = 0;
	virtual void Rotate2Right180AI(float InAngleRate) = 0;

	virtual void EscapeRotateLeft90AI(float InAngleRate) = 0;
	virtual void EscapeRotateLeft180AI(float InAngleRate) = 0;
	virtual void EscapeRotateRight90AI(float InAngleRate) = 0;
	virtual void EscapeRotateRight180AI(float InAngleRate) = 0;
	virtual void EscapeMoveForwardAI() = 0;

	virtual void NoDetectMoveForwardAI(float InDistanceRate) = 0;
	virtual void DetectMoveForwardAI(float InDistanceRate) = 0;

	virtual void RoarAI() = 0;
	virtual void AngerRoarAI() = 0;
	virtual void EscapeRoarAI() = 0;

	virtual void DragonRideReadyAI() = 0;
	virtual void TurfWarReadyAI() = 0;

	virtual void StopMontage() = 0;

	virtual bool IsHp75AI() = 0;
	virtual bool Get270secElapsedAI() = 0;
};
