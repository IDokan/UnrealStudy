// Fill out your copyright notice in the Description page of Project Settings.


#include "LSPlayerController.h"
#include "LSHUDWidget.h"
#include "LSPlayerState.h"
#include "LSCharacter.h"
#include "LSGameplayWidget.h"
#include "LSGameplayResultWidget.h"
#include "LSGameState.h"

ALSPlayerController::ALSPlayerController()
{
	static ConstructorHelpers::FClassFinder<ULSHUDWidget> UI_HUD_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Book/UI/UI_HUD.UI_HUD_C'"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}


	// Input Settings
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IM_InputMapping(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IM_PlayerController.IM_PlayerController'"));
	if (IM_InputMapping.Succeeded())
	{
		InputMapping = IM_InputMapping.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> IA_GamePause(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_GamePause.IA_GamePause'"));
	if (IA_GamePause.Succeeded())
	{
		InputGamePause = IA_GamePause.Object;
	}

	static ConstructorHelpers::FClassFinder<ULSGameplayWidget> UI_MENU_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Book/UI/UI_Menu.UI_Menu_C'"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<ULSGameplayResultWidget> UI_RESULT_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Book/UI/UI_Result.UI_Result_C'"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
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

void ALSPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ALSPlayerController::ShowResultUI()
{
	auto LSGameState = Cast<ALSGameState>(UGameplayStatics::GetGameState(this));
	LSCHECK(LSGameState != nullptr);
	ResultWidget->BindGameState(LSGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}

void ALSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);
	PEI->BindAction(InputGamePause, ETriggerEvent::Triggered, this, &ALSPlayerController::OnGamePause);
}

void ALSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(true);

	HUDWidget = CreateWidget<ULSHUDWidget>(this, HUDWidgetClass);
	LSCHECK(HUDWidget != nullptr);
	HUDWidget->AddToViewport(1);

	ResultWidget = CreateWidget<ULSGameplayResultWidget>(this, ResultWidgetClass);
	LSCHECK(ResultWidget != nullptr);

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

void ALSPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<ULSGameplayWidget>(this, MenuWidgetClass);
	LSCHECK(MenuWidget != nullptr);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}