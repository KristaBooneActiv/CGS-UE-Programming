// Copyright Epic Games, Inc. All Rights Reserved.

#include "MazeGameGameMode.h"
#include "MazeGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMazeGameGameMode::AMazeGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
