// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfectedCityGameMode.h"
#include "InfectedCityCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInfectedCityGameMode::AInfectedCityGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BluePrints/InteractSample/BP_ThirdPersonCustom"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
