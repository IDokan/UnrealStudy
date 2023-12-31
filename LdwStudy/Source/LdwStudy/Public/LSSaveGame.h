// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "GameFramework/SaveGame.h"
#include "LSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LDWSTUDY_API ULSSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	ULSSaveGame();

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Exp;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 HighScore;

	UPROPERTY()
	int32 CharacterIndex;
};
