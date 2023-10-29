// Fill out your copyright notice in the Description page of Project Settings.


#include "LSPlayerController.h"
#include "LSHUDWidget.h"
#include "LSPlayerState.h"
#include "LSCharacter.h"

ALSPlayerController::ALSPlayerController()
{
	static ConstructorHelpers::FClassFinder<ULSHUDWidget> UI_HUD_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Book/UI/UI_HUD.UI_HUD_C'"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

void ALSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LSLOG_S(Warning);
}

void ALSPlayerController::OnPossess(APawn* aPawn)
{
	LSLOG_S(Warning);
	Super::OnPossess(aPawn);
}

void ALSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<ULSHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	LSPlayerState = Cast<ALSPlayerState>(PlayerState);
	LSCHECK(LSPlayerState != nullptr);
	HUDWidget->BindPlayerState(LSPlayerState);
	LSPlayerState->OnPlayerStateChanged.Broadcast();
}

ULSHUDWidget* ALSPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void ALSPlayerController::NPCKill(class ALSCharacter* KilledNPC) const
{
	LSPlayerState->AddExp(KilledNPC->GetExp());
}

void ALSPlayerController::AddGameScore() const
{
	LSPlayerState->AddGameScore();
}