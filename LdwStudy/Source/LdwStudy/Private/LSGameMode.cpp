// Copyright Epic Games, Inc. All Rights Reserved.

#include "LSGameMode.h"
#include "LSCharacter.h"
#include "LSPlayerController.h"
#include "LSPlayerState.h"
#include "LSGameState.h"

ALSGameMode::ALSGameMode()
{
	DefaultPawnClass = ALSCharacter::StaticClass();
	PlayerControllerClass = ALSPlayerController::StaticClass();
	PlayerStateClass = ALSPlayerState::StaticClass();
	GameStateClass = ALSGameState::StaticClass();
}

void ALSGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LSGameState = Cast<ALSGameState>(GameState);
}

void ALSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto LSPlayerState = Cast<ALSPlayerState>(NewPlayer->PlayerState);
	LSCHECK(LSPlayerState != nullptr);
	LSPlayerState->InitPlayerData();
}

void ALSGameMode::AddScore(class ALSPlayerController* ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto LSPlayerController = Cast<ALSPlayerController>(It->Get());
		if ((LSPlayerController != nullptr) && (ScoredPlayer == LSPlayerController))
		{
			LSPlayerController->AddGameScore();
			break;
		}
	}

	LSGameState->AddGameScore();
}