// Fill out your copyright notice in the Description page of Project Settings.


#include "LSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ALSAIController::HomePosKey(TEXT("HomePos"));
const FName ALSAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ALSAIController::TargetKey(TEXT("Target"));

ALSAIController::ALSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/Book/AI/BB_LSCharacter.BB_LSCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/Book/AI/BT_LSCharacter.BT_LSCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ALSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	auto BlackboardPointerAgent = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPointerAgent))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			LSLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
	// Update any changes on the pointer after executing UseBlackboard function.
	this->Blackboard = BlackboardPointerAgent;
}