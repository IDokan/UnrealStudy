// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "GameFramework/Actor.h"
#include "LSWeapon.generated.h"

UCLASS()
class LDWSTUDY_API ALSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALSWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

};
