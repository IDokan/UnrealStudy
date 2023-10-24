// Copyright Epic Games, Inc. All Rights Reserved.

#include "LSGameMode.h"
#include "LSCharacter.h"
#include "LSPlayerController.h"

ALSGameMode::ALSGameMode()
{
	DefaultPawnClass = ALSCharacter::StaticClass();
	PlayerControllerClass = ALSPlayerController::StaticClass();

	// @@ Not going to use in further examples.
	//static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C'"));
	//if (BP_PAWN_C.Succeeded())
	//{
	//	DefaultPawnClass = BP_PAWN_C.Class;
	//}
}

void ALSGameMode::PostLogin(APlayerController* NewPlayer)
{
	LSLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	LSLOG(Warning, TEXT("PostLogin End"));
}