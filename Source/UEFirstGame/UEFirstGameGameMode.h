// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPlayer.h"
#include "UEFirstGameGameMode.generated.h"

UCLASS(minimalapi)
class AUEFirstGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TSubclassOf<AMyPlayer> CustomPlayer = AMyPlayer::StaticClass();

public:
	AUEFirstGameGameMode();
};



