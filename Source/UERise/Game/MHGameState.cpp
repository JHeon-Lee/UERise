// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MHGameState.h"
#include "GameData/MHMacros.h"
#include "Net/UnrealNetwork.h"


void AMHGameState::HandleBeginPlay()
{
	MH_LOG(TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	MH_LOG(TEXT("%s"), TEXT("End"));

}

void AMHGameState::OnRep_ReplicatedHasBegunPlay()
{
	MH_LOG(TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	MH_LOG(TEXT("%s"), TEXT("End"));

}

AMHGameState::AMHGameState()
{
	RemainingTime = MatchPlayTime;
}

void AMHGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMHGameState, RemainingTime);
}
