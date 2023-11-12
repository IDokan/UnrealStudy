// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "LSGameplayWidget.h"
#include "LSGameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class LDWSTUDY_API ULSGameplayResultWidget : public ULSGameplayWidget
{
	GENERATED_BODY()
	
public:
	void BindGameState(class ALSGameState* GameState);
	
protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class ALSGameState> CurrentGameState;
};
