// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSpaceShipGameMode.h"
#include "TPSpaceShipCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPSpaceShipGameMode::ATPSpaceShipGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/MyVaisseau"));
	
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
