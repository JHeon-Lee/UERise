// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MH_PlayerStatComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMH_PlayerStatComponent::UMH_PlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	MaxHp = 100.0f;
	MaxHpModifier = 0.0f;
	CurrentHp = 100.0f;
	MaxStamina = 200.0f;
	MaxStaminaModifier = 0.0f;
	CurrentStamina = 200.0f;
	MaxWeaponDurability = 100.0f;
	CurrentWeaponDurability = 100.0f;
	Atk = 100.0f;
	AtkModifier = 0.0f;
	Critical = 30.0f;
}


// Called when the game starts
void UMH_PlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
}

float UMH_PlayerStatComponent::CaculateDamage(float BasePower, bool &IsCriticalHit)
{
	float Damage = BasePower * (Atk + AtkModifier) * (CurrentWeaponDurability / MaxWeaponDurability);
	switch (ChargeStep)
	{
	case 1:	Damage *= 1.1f; break;
	case 2:	Damage *= 1.5f; break;
	case 3:	Damage *= 2.0f; break;
	}
	
	const float RandomValue = UKismetMathLibrary::RandomFloatInRange(0.0, 100.0);
	
	if (Critical > RandomValue)
	{
		Damage *= 1.4f;
		IsCriticalHit = true;
	}

	return Damage;
}
