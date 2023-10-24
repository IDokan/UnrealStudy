// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCharacterWidget.h"
#include "LSCharacterStatComponent.h"
#include "Components/ProgressBar.h"


void ULSCharacterWidget::BindCharacterStat(ULSCharacterStatComponent* NewCharacterStat)
{
	LSCHECK(NewCharacterStat != nullptr);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &ULSCharacterWidget::UpdateHPWidget);
}

void ULSCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	LSCHECK(HPProgressBar != nullptr);
	UpdateHPWidget();
}

void ULSCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (HPProgressBar != nullptr)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}

