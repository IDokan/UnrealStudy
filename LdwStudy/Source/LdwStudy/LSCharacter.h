// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LdwStudy.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "LSCharacter.generated.h"

UCLASS()
class LDWSTUDY_API ALSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO,
		MAX_MODE,
	};

	void SetControlMode(EControlMode ControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.f;
	float ArmRotationSpeed = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* inputUpDown;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* inputLeftRight;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* inputLookUp;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* inputTurn;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* inputViewChange;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* inputUpDownReleased;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* inputLeftRightReleased;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* inputJump;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* inputAttack;

private:
	void UpDown(const FInputActionValue& NewAxisValue);
	void LeftRight(const FInputActionValue& NewAxisValue);
	void UpDownReleased();
	void LeftRightReleased();

	void LookUp(const FInputActionValue& NewAxisValue);
	void Turn(const FInputActionValue& NewAxisValue);

	void Attack();

	void ViewChange();


	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void AttackStartComboState();
	void AttackEndComboState();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY()
	class ULSAnimInstance* LSAnim;
};
