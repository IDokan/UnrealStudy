// Fill out your copyright notice in the Description page of Project Settings.


#include "LSPawn.h"

// Sets default values
ALSPawn::ALSPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALSPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALSPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LSLOG_S(Warning);
}

void ALSPawn::PossessedBy(AController* NewController)
{
	LSLOG_S(Warning);
	Super::PossessedBy(NewController);
}
