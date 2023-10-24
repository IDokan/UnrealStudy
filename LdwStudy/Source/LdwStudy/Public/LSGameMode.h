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


	virtual void PostLogin(APlayerController* newPlayer) override;
};
