// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MH_AnimNotifyInterface.generated.h"

/**키 입력 정보 Enum*/
UENUM(BlueprintType)
enum class EKeyInfo : uint8
{
	Default			UMETA(DisplayName = "Default"),
	LT_Y			UMETA(DisplayName = "LT+Y"),
	LT_B			UMETA(DisplayName = "LT+B"),
	Y_B				UMETA(DisplayName = "Y+B"),
	WASD_Y			UMETA(DisplayName = "WASD+Y"),
	Y				UMETA(DisplayName = "Y"),
	Release_Y		UMETA(DisplayName = "Release_Y"),
	B				UMETA(DisplayName = "B"),
	Release_B		UMETA(DisplayName = "Release_B"),
	Space			UMETA(DisplayName = "Space"),
	MovingOnGround	UMETA(DisplayName = "MovingOnGround"),
	HoldingWeapon	UMETA(DisplayName = "HoldingWeapon"),
	Unarmed			UMETA(DisplayName = "Unarmed"),
	LT_A			UMETA(DisplayName = "LT_A"),
	WASD			UMETA(DisplayName = "WASD"),
	HuntingEdgeHit	UMETA(DisplayName = "HuntingEdgeHit"),
	RT				UMETA(DisplayName = "RT"),
	Release_RT		UMETA(DisplayName = "Release_RT"),
	NearWall		UMETA(DisplayName = "NearWall"),
	NotNearWall		UMETA(DisplayName = "NotNearWall"),
	Release_RB		UMETA(DisplayName = "Release_RB")
};

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
	virtual void SwdAttachToSocket(FName socketName) = 0;

	virtual void CameraShake(bool IsStrong = false) = 0;

	virtual void GSwdFstCharge() = 0;
	virtual void GSwdSndCharge() = 0;
	virtual void GSwdTrdCharge() = 0;

	virtual void TurnOnBuffEffect() = 0;


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

};
