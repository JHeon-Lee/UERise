// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/UtusiStatComponent.h"

// Sets default values for this component's properties
UUtusiStatComponent::UUtusiStatComponent()
{
	
	MaxHp = 200.0f;
	SetHp(MaxHp);

}


float UUtusiStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHP;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	SetHp(PrevHp - ActualDamage);
	if (CurrentHP <= 0.1f)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UUtusiStatComponent::SetHp(float NewHp)
{
	CurrentHP = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	OnHpChanged.Broadcast(CurrentHP);

}

// Called when the game starts
void UUtusiStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHp;

	
}
