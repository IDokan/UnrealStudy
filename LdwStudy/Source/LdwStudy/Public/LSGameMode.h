// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "GameFramework/GameModeBase.h"
#include "LSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LDWSTUDY_API ALSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALSGameMode();

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* newPlayer) override;
	void AddScore(class ALSPlayerController* ScoredPlayer);

private:
	UPROPERTY()
	class ALSGameState* LSGameState;
};
