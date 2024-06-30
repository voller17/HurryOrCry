// Copyright Epic Games, Inc. All Rights Reserved.

#include "HurryOrCryGameMode.h"
#include "HurryOrCryCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHurryOrCryGameMode::AHurryOrCryGameMode()
{
	// set default pawn class to our Blueprinted character
	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
}
