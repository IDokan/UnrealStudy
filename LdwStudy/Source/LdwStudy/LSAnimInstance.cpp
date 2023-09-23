// Fill out your copyright notice in the Description page of Project Settings.


#include "LSAnimInstance.h"

ULSAnimInstance::ULSAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsInAir = false;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> WARRIOR_ANIM_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage'"));
	if (WARRIOR_ANIM_MONTAGE.Succeeded())
	{
		AttackMontage = (WARRIOR_ANIM_MONTAGE.Object);
	}
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

void ULSAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void ULSAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	LSCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void ULSAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void ULSAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName ULSAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	LSCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
