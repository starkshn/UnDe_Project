// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnDeGameMode.h"
#include "UnDeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnDeGameMode::AUnDeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
