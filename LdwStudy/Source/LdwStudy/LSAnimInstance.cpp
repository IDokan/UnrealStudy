// Fill out your copyright notice in the Description page of Project Settings.


#include "LSAnimInstance.h"

ULSAnimInstance::ULSAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsInAir = false;
}

// Game engine :: Input -> Game Logic -> Animation System
void ULSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (::IsValid(Pawn))
	{
		// Update Pawn Speed
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		// Update ground state
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}