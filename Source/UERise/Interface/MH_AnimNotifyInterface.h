// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData/MHGlobalEnum.h"
#include "MH_AnimNotifyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMH_AnimNotifyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UERISE_API IMH_AnimNotifyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TurnOnBuffEffect() = 0;

	virtual void CameraShake(bool IsStrong = false) = 0;

	virtual void ManualMoveBegin() = 0;
	virtual void ManualMoveTick(float ManualMoveSpeed, float ManualMoveZSpeed, float FlymodeTime, bool WallRun) = 0;
	virtual void ManualMoveEnd() = 0;

	virtual void ShootWirebugBegin(FName SocketName) = 0;
	virtual void ShootWirebugTick(float Distance, float ZDistance, float FDistance, float Movetime) = 0;
	virtual void ShootWirebugEnd() = 0;

	virtual void ComboTick(TMap<EKeyInfo, TObjectPtr<class UAnimMontage>> MontageMap,FName SectionName) = 0;
	virtual void ComboEnd(bool IsChargeAtk) = 0;

	virtual void AttackBegin() = 0;
	virtual void AttackTick(FName AtkStartSocket, FName AtkEndSocket, float AtkRadius) = 0;

	virtual void DamageTakeBegin(EDamageTakeType DmgTakeType) = 0;
	virtual void DamageTakeEnd() = 0;

	virtual void ValutBegin(float Offset) = 0;
	virtual void ValutEnd() = 0;

	virtual void RotateTick(float InitialYaw, float MaxRotateDegree, float RotateSpeed) = 0;

};
