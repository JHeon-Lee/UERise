// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MHGameMode.h"

AMHGameMode::AMHGameMode()
{

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/ Script / Engine.Blueprint'/Game/Characters/Utusi/Bluprints/RealUtusi.RealUtusi_C'"));
	
	
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/UERise.MHPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
