// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MHGameState.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API AMHGameState : public AGameStateBase
{
	GENERATED_BODY()
	

public	:
	virtual void HandleBeginPlay() override;

	virtual void OnRep_ReplicatedHasBegunPlay() override;

	AMHGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;


	UPROPERTY(Transient, Replicated)
	int32 RemainingTime;

	int32 MatchPlayTime = 2000;
	int32 ShowResultWaitingTime = 5;

};
