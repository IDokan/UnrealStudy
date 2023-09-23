// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCharacter.h"
#include "LSAnimInstance.h"

// Sets default values
ALSCharacter::ALSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Script/Engine.AnimBlueprint'/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C'"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	// Input Settings
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IM_InputMapping(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IM_Default.IM_Default'"));
	if (IM_InputMapping.Succeeded())
	{
		InputMapping = IM_InputMapping.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_UpDown(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_UpDown.IA_UpDown'"));
	if (IA_UpDown.Succeeded())
	{
		inputUpDown = IA_UpDown.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LeftRight(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_LeftRight.IA_LeftRight'"));
	if (IA_LeftRight.Succeeded())
	{
		inputLeftRight = IA_LeftRight.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LookUp(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_LookUp.IA_LookUp'"));
	if (IA_LookUp.Succeeded())
	{
		inputLookUp = IA_LookUp.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Turn(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Turn.IA_Turn'"));
	if (IA_Turn.Succeeded())
	{
		inputTurn = IA_Turn.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ViewChange(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_ViewChange.IA_ViewChange'"));
	if (IA_Turn.Succeeded())
	{
		inputViewChange = IA_ViewChange.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_UpDown_Released(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_UpDown_Released.IA_UpDown_Released'"));
	if (IA_UpDown_Released.Succeeded())
	{
		inputUpDownReleased = IA_UpDown_Released.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LeftRight_Released(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_LeftRight_Released.IA_LeftRight_Released'"));
	if (IA_LeftRight_Released.Succeeded())
	{
		inputLeftRightReleased = IA_LeftRight_Released.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Jump(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Jump.IA_Jump'"));
	if (IA_Jump.Succeeded())
	{
		inputJump = IA_Jump.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Attack.IA_Attack'"));
	if (IA_Attack.Succeeded())
	{
		inputAttack = IA_Attack.Object;
	}

	SetControlMode(EControlMode::DIABLO);

	ArmLengthSpeed = 3.f;
	ArmRotationSpeed = 10.f;
	GetCharacterMovement()->JumpZVelocity = 800.f;

	IsAttacking = false;

	MaxCombo = 4;
	AttackEndComboState();
}

// Called when the game starts or when spawned
void ALSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALSCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (NewControlMode)
	{
	case EControlMode::GTA:
		ArmLengthTo = 450.f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	case EControlMode::DIABLO:
		ArmLengthTo = 800.f;
		ArmRotationTo = FRotator(-45.f, 0.f, 0.f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	default:
		break;
	}
}

// Called every frame
void ALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		if (DirectionToMove.SizeSquared() > 0.f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	default:
		break;
	}
}

void ALSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LSAnim = Cast <ULSAnimInstance>(GetMesh()->GetAnimInstance());
	LSCHECK(LSAnim != nullptr);

	LSAnim->OnMontageEnded.AddDynamic(this, &ALSCharacter::OnAttackMontageEnded);

	LSAnim->OnNextAttackCheck.AddLambda(
		[this]()->void {
			LSLOG(Warning, TEXT("OnNextAttackCheck"));
			CanNextCombo = false;

			if (IsComboInputOn)
			{
				AttackStartComboState();
				LSAnim->JumpToAttackMontageSection(CurrentCombo);
			}
		}
	);
}

// Called to bind functionality to input
void ALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(inputUpDown, ETriggerEvent::Triggered, this, &ALSCharacter::UpDown);
	PEI->BindAction(inputLeftRight, ETriggerEvent::Triggered, this, &ALSCharacter::LeftRight);
	PEI->BindAction(inputLookUp, ETriggerEvent::Triggered, this, &ALSCharacter::LookUp);
	PEI->BindAction(inputTurn, ETriggerEvent::Triggered, this, &ALSCharacter::Turn);
	PEI->BindAction(inputViewChange, ETriggerEvent::Triggered, this, &ALSCharacter::ViewChange);
	PEI->BindAction(inputUpDownReleased, ETriggerEvent::Triggered, this, &ALSCharacter::UpDownReleased);
	PEI->BindAction(inputLeftRightReleased, ETriggerEvent::Triggered, this, &ALSCharacter::LeftRightReleased);
	PEI->BindAction(inputJump, ETriggerEvent::Triggered, this, &ALSCharacter::Jump);
	PEI->BindAction(inputAttack, ETriggerEvent::Triggered, this, &ALSCharacter::Attack);
}



void ALSCharacter::UpDown(const FInputActionValue& NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue.Get<float>());
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue.Get<float>();
		break;
	default:
		break;
	}
}

void ALSCharacter::LeftRight(const FInputActionValue& NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue.Get<float>());
	break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue.Get<float>();
		break;
	default:
		break;
	}
}

void ALSCharacter::LookUp(const FInputActionValue& NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue.Get<float>());
		break;
	default:
		break;
	}
}

void ALSCharacter::Turn(const FInputActionValue& NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue.Get<float>());
		break;
	default:
		break;
	}
}

void ALSCharacter::Attack()
{
	if (IsAttacking)
	{
		LSCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		LSCHECK(CurrentCombo == 0);
		AttackStartComboState();
		LSAnim->PlayAttackMontage();
		LSAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void ALSCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);
		break;
	default:
		break;
	}
}

void ALSCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	LSCHECK(IsAttacking);
	LSCHECK(CurrentCombo > 0);

	IsAttacking = false;
	AttackEndComboState();
}

void ALSCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	LSCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ALSCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ALSCharacter::UpDownReleased()
{
	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		DirectionToMove.X = 0.f;
		break;
	default:
		break;
	}
}

void ALSCharacter::LeftRightReleased()
{
	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		DirectionToMove.Y = 0.f;
		break;
	default:
		break;
	}
}

