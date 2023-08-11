// Copyright Epic Games, Inc. All Rights Reserved.

#include "LdwStudyGameMode.h"
#include "LdwStudyCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALdwStudyGameMode::ALdwStudyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
