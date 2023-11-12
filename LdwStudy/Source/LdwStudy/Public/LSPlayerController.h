// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "LSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LDWSTUDY_API ALSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALSPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	class ULSHUDWidget* GetHUDWidget() const;
	void NPCKill(class ALSCharacter* KilledNPC) const;
	void AddGameScore() const;


	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InputGamePause;

	void ChangeInputMode(bool bGameMode = true);

	void ShowResultUI();

protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class ULSHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class ULSGameplayWidget> MenuWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class ULSGameplayResultWidget> ResultWidgetClass;

private:
	void OnGamePause();

	UPROPERTY()
	class ULSHUDWidget* HUDWidget;

	UPROPERTY()
	class ALSPlayerState* LSPlayerState;

	UPROPERTY()
	class ULSGameplayWidget* MenuWidget;

	UPROPERTY()
	class ULSGameplayResultWidget* ResultWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};
