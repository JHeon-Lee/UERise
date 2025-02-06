// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/MHGameMode.h"
#include "GameData/MHMacros.h"
#include "Engine/Public/EngineUtils.h"
#include "GameFramework/PlayerStart.h"

AMHGameMode::AMHGameMode()
{

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/ Script / Engine.Blueprint'/Game/Characters/Utusi/Bluprints/RealUtusi.RealUtusi_C'"));
	
	
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/Blueprints/BP_MHPlayerController.BP_MHPlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void AMHGameMode::OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn)
{
}

void AMHGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{	
	MH_LOG(TEXT("%s"), TEXT("Begin"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	MH_LOG(TEXT("%s"), TEXT("End"));
}

APlayerController* AMHGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	MH_LOG(TEXT("%s"), TEXT("Begin"));
	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	MH_LOG(TEXT("%s"), TEXT("End"));

	return NewPlayerController;
}

void AMHGameMode::PostLogin(APlayerController* NewPlayer)
{
	MH_LOG(TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		if (NetDriver->ClientConnections.Num() == 0)
		{
			MH_LOG(TEXT("%s"), TEXT("No Client Connection"));
		}
		else
		{
			for (const auto& Connection : NetDriver->ClientConnections)
			{
				MH_LOG(TEXT("Client Connections: %s"), *Connection->GetName());
			}
		}
	}
	else
	{
		MH_LOG( TEXT("%s"), TEXT("No NetDriver"));
	}
	MH_LOG(TEXT("%s"), TEXT("End"));

}

void AMHGameMode::StartPlay()
{
	Super::StartPlay();

	for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
	{
		PlayerStartArray.Add(PlayerStart);
	}
}

void AMHGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}