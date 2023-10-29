#include "LSCharacterStatComponent.h"
#include "LSCharacterStatComponent.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCharacterStatComponent.h"
#include "LSGameInstance.h"

// Sets default values for this component's properties
ULSCharacterStatComponent::ULSCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void ULSCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void ULSCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void ULSCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	// @@ TODO:: How to get Game Instance? Below method probably does not more provided in Unreal Engine 5.1..
	auto LSGameInstance = Cast<ULSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	LSCHECK(LSGameInstance != nullptr);
	CurrentStatData = LSGameInstance->GetLSCharacterData(NewLevel);
	if (CurrentStatData != nullptr)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		LSLOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void ULSCharacterStatComponent::SetDamage(float NewDamage)
{
	LSCHECK(CurrentStatData != nullptr);

	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.f, CurrentStatData->MaxHP));
}

void ULSCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnHPIsZero.Broadcast();
	}
}

float ULSCharacterStatComponent::GetAttack()
{
	LSCHECK(CurrentStatData != nullptr, 0.f);
	return CurrentStatData->Attack;
}

float ULSCharacterStatComponent::GetHPRatio()
{
	LSCHECK(CurrentStatData != nullptr, 0.f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.f : (CurrentHP / CurrentStatData->MaxHP);
}

int32 ULSCharacterStatComponent::GetDropExp()
{
	return CurrentStatData->DropExp;
}