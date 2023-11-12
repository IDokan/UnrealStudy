// Fill out your copyright notice in the Description page of Project Settings.


#include "LSGameplayResultWidget.h"
#include "Components/TextBlock.h"
#include "LSGameState.h"

void ULSGameplayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LSCHECK(CurrentGameState.IsValid());

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	LSCHECK(Result != nullptr);
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Mission Complete") : TEXT("Mission Failed")));

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	LSCHECK(TotalScore != nullptr);
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}

void ULSGameplayResultWidget::BindGameState(class ALSGameState* GameState)
{
	LSCHECK(GameState != nullptr);
	CurrentGameState = GameState;
}