// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

UCLASS()
class LDWSTUDY_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Water;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Splash;

	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

private:
	UPROPERTY(EditAnywhere, Category=Stat, Meta=(AllowPrivateAccess=true))
	float RotateSpeed;
};

// UPROPERTY is a macro to enable automatic garbage collection.
// The above macro can be used on only Unreal Objects
// In order to satisfy conditions of Unreal Objects, you need to follow below rules.
	// 1. Class declaration macro
		// There should be a UCLASS() macro and a GENERATED_BODY() macro in class body.
	// 2. CLASS name sufix
		// A name of a class should starts U or A. A is for an actor class, U is for anyother classes not an actor class.
	// 3. generated.h header file
		// In order to allow Unreal engine inspect class declarations, we need let Unreal Header Tool control it by including generated.h.
	// 4. scope of external modules
		// Need [ModuleName]_API keyword before class name to provide access to all other modules.