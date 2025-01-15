// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MH_PlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHpChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHpModifierChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHpChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaModifierChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentStaminaChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxWeaponDurabilityChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponDurabilityChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAtkChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAtkModifierChangedDelegate, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCriticalChangedDelegate, float /*ChagedValue*/);

#define VALUE_GETTER(PropertyName) \
	FORCEINLINE float Get##PropertyName() const { return PropertyName; }

#define VALUE_SETTER(PropertyName) \
	FORCEINLINE void Set##PropertyName(float NewVal) { PropertyName = NewVal; On##PropertyName##Changed.Broadcast(NewVal); }


#define VALUE_BASICFUNCTIONS(PropertyName) \
	FOn##PropertyName##ChangedDelegate On##PropertyName##Changed;\
	VALUE_GETTER(PropertyName) \
	VALUE_SETTER(PropertyName) \


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
	VALUE_BASICFUNCTIONS(MaxHp);
	VALUE_BASICFUNCTIONS(MaxHpModifier);
	VALUE_BASICFUNCTIONS(CurrentHp);
	VALUE_BASICFUNCTIONS(MaxStamina);
	VALUE_BASICFUNCTIONS(MaxStaminaModifier);
	VALUE_BASICFUNCTIONS(CurrentStamina);
	VALUE_BASICFUNCTIONS(MaxWeaponDurability);
	VALUE_BASICFUNCTIONS(CurrentWeaponDurability);
	VALUE_BASICFUNCTIONS(Atk);
	VALUE_BASICFUNCTIONS(AtkModifier);
	VALUE_BASICFUNCTIONS(Critical);

public:
	float CaculateDamage(float BasePower, bool &IsCriticalHit);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHpModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStaminaModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxWeaponDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentWeaponDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AtkModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Critical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 WireBugStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 CurWireBugStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 ChargeStep;

};

