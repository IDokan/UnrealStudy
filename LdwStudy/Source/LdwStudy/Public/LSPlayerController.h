// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "GameFramework/PlayerController.h"
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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class ULSHUDWidget> HUDWidgetClass;

private:
	UPROPERTY()
	class ULSHUDWidget* HUDWidget;

	UPROPERTY()
	class ALSPlayerState* LSPlayerState;
};
