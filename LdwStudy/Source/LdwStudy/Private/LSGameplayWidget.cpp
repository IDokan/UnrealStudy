// Fill out your copyright notice in the Description page of Project Settings.


#include "LSGameplayWidget.h"
#include "Components/Button.h"
#include "LSPlayerController.h"

void ULSGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (ResumeButton != nullptr)
	{
		ResumeButton->OnClicked.AddDynamic(this, &ULSGameplayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (ReturnToTitleButton != nullptr)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &ULSGameplayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (RetryGameButton != nullptr)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &ULSGameplayWidget::OnRetryGameClicked);
	}
}

void ULSGameplayWidget::OnResumeClicked()
{
	auto LSPlayerController = Cast<ALSPlayerController>(GetOwningPlayer());
	LSCHECK(LSPlayerController);

	RemoveFromParent();

	LSPlayerController->ChangeInputMode(true);
	LSPlayerController->SetPause(false);
}


void ULSGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}


void ULSGameplayWidget::OnRetryGameClicked()
{
	auto LSPlayerController = Cast<ALSPlayerController>(GetOwningPlayer());
	LSCHECK(LSPlayerController);
	LSPlayerController->RestartLevel();
}
