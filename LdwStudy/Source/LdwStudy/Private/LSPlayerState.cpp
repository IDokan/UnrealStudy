// Fill out your copyright notice in the Description page of Project Settings.


#include "LSPlayerState.h"
#include "LSGameInstance.h"
#include "LSSaveGame.h"

ALSPlayerState::ALSPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 0;
	Exp = 0;
	SaveSlotName = TEXT("Player1");
}

int32 ALSPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ALSPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

float ALSPlayerState::GetExpRatio() const
{
	if (CurrentStatData == nullptr)
	{
		LSLOG(Error, TEXT("CurrentStatData is nullptr"));
		return 0.f;
	}
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
	{
		return 0.f;
	};

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	LSLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

bool ALSPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
	{
		return false;
	}

	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();
	SavePlayerData();

	return DidLevelUp;
}

void ALSPlayerState::InitPlayerData()
{
	auto LSSaveGame = Cast<ULSSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (LSSaveGame == nullptr)
	{
		LSSaveGame = GetMutableDefault<ULSSaveGame>();
	}

	SetPlayerName(LSSaveGame->PlayerName);
	SetCharacterLevel(LSSaveGame->Level);
	GameScore = 0;
	GameHighScore = LSSaveGame->HighScore;
	Exp = LSSaveGame->Exp;
	SavePlayerData();
}

void ALSPlayerState::SavePlayerData()
{
	ULSSaveGame* NewPlayerData = NewObject <ULSSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		LSLOG(Error, TEXT("SaveGame Error!"));
	}

	// Leave allocated new memory to be handled by GarbageCollectors
}

void ALSPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto tmp = GetGameInstance();
	auto LSGameInstance = Cast<ULSGameInstance>(GetGameInstance());
	LSCHECK(LSGameInstance != nullptr);

	CurrentStatData = LSGameInstance->GetLSCharacterData(NewCharacterLevel);
	LSCHECK(CurrentStatData != nullptr);

	CharacterLevel = NewCharacterLevel;
}

void ALSPlayerState::AddGameScore()
{
	++GameScore;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

int32 ALSPlayerState::GetGameHighScore() const
{

	return GameHighScore;
}
