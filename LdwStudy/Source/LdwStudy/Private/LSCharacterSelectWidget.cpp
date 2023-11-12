// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCharacterSelectWidget.h"
#include "LSCharacterSetting.h"
#include "LSGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "LSSaveGame.h"
#include "LSPlayerState.h"

void ULSCharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	if (CurrentIndex == -1)
	{
		CurrentIndex = MaxIndex - 1;
	}
	if (CurrentIndex == MaxIndex)
	{
		CurrentIndex = 0;
	}

	auto CharacterSetting = GetDefault<ULSCharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto LSGameInstance = GetWorld()->GetGameInstance<ULSGameInstance>();
	LSCHECK(LSGameInstance != nullptr);
	LSCHECK(TargetComponent.IsValid());

	USkeletalMesh* Asset = LSGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (Asset != nullptr)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

void ULSCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<ULSCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	LSCHECK(PrevButton != nullptr);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	LSCHECK(NextButton != nullptr);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	LSCHECK(TextBox != nullptr);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	LSCHECK(ConfirmButton != nullptr);

	PrevButton->OnClicked.AddDynamic(this, &ULSCharacterSelectWidget::OnPrevClicked);

	NextButton->OnClicked.AddDynamic(this, &ULSCharacterSelectWidget::OnNextClicked);

	ConfirmButton->OnClicked.AddDynamic(this, &ULSCharacterSelectWidget::OnConfirmClicked);
}

void ULSCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void ULSCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void ULSCharacterSelectWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10)
	{
		return;
	}

	ULSSaveGame* NewPlayerData = NewObject <ULSSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	auto LSPlayerState = GetDefault<ALSPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, LSPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GamePlay"));
	}
	else
	{
		LSLOG(Error, TEXT("SaveGame Error!"));
	}
}
