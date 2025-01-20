#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MHGlobalEnum.generated.h"

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
	Release_RB		UMETA(DisplayName = "Release_RB"), 
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDamageTakeType : uint8
{
	Default UMETA(DisplayName = "Default"),
	Evade UMETA(DisplayName = "Evade"),
	Gaurd UMETA(DisplayName = "Gaurd"),
	Tackle UMETA(DisplayName = "Tackle"),
};


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	GreatSwdArmed UMETA(DisplayName = "Armed"),
	LongSwdArmed UMETA(DisplayName = "LongSwdArmed"),
	Riding UMETA(DisplayName = "Riding"),
	DragonRide UMETA(DisplayName = "DragonRide")
};

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	Village UMETA(DisplayName = "Village"),
	Field UMETA(DisplayName = "Field")
};

UENUM(BlueprintType)
enum class EValutMontage : uint8
{
	WallRun UMETA(DisplayName = "WallRun"),
	JumpOver UMETA(DisplayName = "JumpOver"),
	Vault UMETA(DisplayName = "Vault")
};