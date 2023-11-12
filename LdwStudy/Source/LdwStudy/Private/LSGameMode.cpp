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

	ScoreToClear = 2;
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

	if (GetScore() >= ScoreToClear)
	{
		LSGameState->SetGameCleared();

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto LSPlayerController = Cast<ALSPlayerController>(It->Get());
			if (LSPlayerController != nullptr)
			{
				LSPlayerController->ShowResultUI();
			}
		}
	}
}

int32 ALSGameMode::GetScore() const
{
	return LSGameState->GetTotalGameScore();
}