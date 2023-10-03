// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "LSAIController.h"
#include "LSCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	auto Target = Cast<ALSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ALSAIController::TargetKey));
	if (Target == nullptr)
	{
		return false;
	}

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.f);

	return bResult;
}
