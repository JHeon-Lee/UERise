// Copyright Epic Games, Inc. All Rights Reserved.

#include "UERiseGameMode.h"
#include "UERiseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUERiseGameMode::AUERiseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
