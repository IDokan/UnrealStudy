// Fill out your copyright notice in the Description page of Project Settings.


#include "LSPlayerState.h"
#include "LSGameInstance.h"

ALSPlayerState::ALSPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
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

	return DidLevelUp;
}

void ALSPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	SetCharacterLevel(5);
	GameScore = 0;
	Exp = 0;
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
	OnPlayerStateChanged.Broadcast();
}