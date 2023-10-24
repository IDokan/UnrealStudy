// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "AIController.h"
#include "LSAIController.generated.h"

/**
 * 
 */
UCLASS()
class LDWSTUDY_API ALSAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ALSAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
