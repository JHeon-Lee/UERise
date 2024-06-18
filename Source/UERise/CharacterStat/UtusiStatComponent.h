// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UtusiStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*hp*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UERISE_API UUtusiStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUtusiStatComponent();

public: //Delegate
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;


public: // Getter
	FORCEINLINE float GetMaxHP() { return MaxHp; }
	FORCEINLINE float GetCurrentHP() { return CurrentHP; }
	float ApplyDamage(float InDamage);

protected: //Setter
	void SetHp(float NewHp);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentHP;



};
