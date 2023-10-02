// Fill out your copyright notice in the Description page of Project Settings.


#include "LSGameInstance.h"

ULSGameInstance::ULSGameInstance()
{
	FString CharacterDataPath = TEXT("/Script/Engine.DataTable'/Game/Book/GameData/LSCharacterData.LSCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable>DT_LSCHARACTER(*CharacterDataPath);
	LSCHECK(DT_LSCHARACTER.Succeeded());
	LSCharacterTable = DT_LSCHARACTER.Object;
	LSCHECK(LSCharacterTable->GetRowMap().Num() > 0);
}

void ULSGameInstance::Init()
{
	Super::Init();
	LSLOG(Warning, TEXT("DropExp of Level 20 LSCharacter : %d"), GetLSCharacterData(20)->DropExp);
}

FLSCharacterData* ULSGameInstance::GetLSCharacterData(int32 Level)
{
	return LSCharacterTable->FindRow<FLSCharacterData>(*FString::FromInt(Level), TEXT(""));
}
