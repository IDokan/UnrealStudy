// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "Blueprint/UserWidget.h"
#include "LSHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class LDWSTUDY_API ULSHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class ULSCharacterStatComponent* CharacterStat);
	void BindPlayerState(class ALSPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();
	
private:
	TWeakObjectPtr<class ULSCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class ALSPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* PlayerName;

	UPROPERTY()
	class UTextBlock* PlayerLevel;

	UPROPERTY()
	class UTextBlock* CurrentScore;

	UPROPERTY()
	class UTextBlock* HighScore;
};
