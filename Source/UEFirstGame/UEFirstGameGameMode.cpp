// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEFirstGameGameMode.h"
#include "UEFirstGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUEFirstGameGameMode::AUEFirstGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
