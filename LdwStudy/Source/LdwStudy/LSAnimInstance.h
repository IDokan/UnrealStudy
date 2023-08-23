// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "Animation/AnimInstance.h"
#include "LSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LDWSTUDY_API ULSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	ULSAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
};
