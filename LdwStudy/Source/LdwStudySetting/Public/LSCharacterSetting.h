// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LSCharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config = LdwStudy)
class LDWSTUDYSETTING_API ULSCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	ULSCharacterSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};
