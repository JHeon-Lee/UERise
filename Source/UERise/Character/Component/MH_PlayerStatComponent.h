// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MHMacros.h"
#include "MH_PlayerStatComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHpChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHpModifierChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentHpChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaModifierChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentStaminaChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxWeaponDurabilityChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponDurabilityChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAtkChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAtkModifierChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCriticalChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWireBugStackChanged, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurWireBugStackChanged, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChargeStepChanged, int32, NewValue);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UERISE_API UMH_PlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMH_PlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, MaxHp);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, MaxHpModifier);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, CurrentHp);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, MaxStamina);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, MaxStaminaModifier);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, CurrentStamina);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, MaxWeaponDurability);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, CurrentWeaponDurability);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, Atk);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, AtkModifier);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(float, Critical);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(int32, WireBugStack);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(int32, CurWireBugStack);
	PROPERTY_WITH_ACCESSORS_AND_DELEGATE(int32, ChargeStep);


public:
	float CaculateDamage(float BasePower, bool &IsCriticalHit);


};

