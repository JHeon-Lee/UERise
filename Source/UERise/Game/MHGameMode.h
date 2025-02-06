// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MHGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API AMHGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMHGameMode();

	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn);

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void StartPlay() override;

protected:
	virtual void PostInitializeComponents() override;

protected:
	TArray<TObjectPtr<class APlayerStart>> PlayerStartArray;
};
