// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "LSGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FLSCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FLSCharacterData() : Level(1), MaxHP(100.f), Attack(10.f), DropExp(10), NextExp(30)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

/**
 * 
 */
UCLASS()
class LDWSTUDY_API ULSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	ULSGameInstance();

	virtual void Init() override;
	FLSCharacterData* GetLSCharacterData(int32 Level);

	FStreamableManager StreamableManager;

private:
	UPROPERTY()
	class UDataTable* LSCharacterTable;
};
