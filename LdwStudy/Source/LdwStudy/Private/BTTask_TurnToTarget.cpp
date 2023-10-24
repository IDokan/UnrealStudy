// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "LSAIController.h"
#include "LSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto LSCharacter = Cast<ALSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (LSCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<ALSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ALSAIController::TargetKey));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - LSCharacter->GetActorLocation();
	LookVector.Z = 0.f;
	
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	LSCharacter->SetActorRotation(FMath::RInterpTo(LSCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.f));

	return EBTNodeResult::Succeeded;
}
