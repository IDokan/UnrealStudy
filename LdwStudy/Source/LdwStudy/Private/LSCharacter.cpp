// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCharacter.h"
#include "LSAnimInstance.h"
#include "LSWeapon.h"
#include "LSCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "LSCharacterWidget.h"
#include "LSAIController.h"
#include "LSCharacterSetting.h"
#include "LSGameInstance.h"

// Sets default values
ALSCharacter::ALSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<ULSCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

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

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("LSCharacter"));

	AttackRange = 200.f;
	AttackRadius = 50.f;

	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/Book/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}

	AIControllerClass = ALSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ALSCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto CharacterWidget = Cast<ULSCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

	if (!IsPlayerControlled())
	{
		auto DefaultSetting = GetDefault<ULSCharacterSetting>();
		int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
		CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];

		auto LSGameInstance = Cast<ULSGameInstance>(GetGameInstance());
		if (LSGameInstance != nullptr)
		{
			AssetStreamingHandle = LSGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &ALSCharacter::OnAssetLoadCompleted));
		}
	}
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
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
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

	// @@ TODO: IS this a line to add pointer this to an actor list ignoring collision check?
	LSAnim->OnAttackHitCheck.AddUObject(this, &ALSCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]()->void 
		{
			LSLOG(Warning, TEXT("OnHPIsZero"));
			LSAnim->SetDeadAnim();
			SetActorEnableCollision(false);
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

bool ALSCharacter::CanSetWeapon()
{
	return (CurrentWeapon == nullptr);
}

void ALSCharacter::SetWeapon(ALSWeapon* NewWeapon)
{
	LSCHECK(NewWeapon != nullptr && CurrentWeapon == nullptr);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (NewWeapon != nullptr)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

float ALSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	LSLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void ALSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::DIABLO);
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
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

	OnAttackEnd.Broadcast();
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

void ALSCharacter::AttackCheck()
{
	FHitResult HitResult;

	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	// From End of sphere to center of cylinder
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 2.5f;

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
#endif

	if (bResult)
	{
		if (AActor* HitActor = HitResult.GetActor();
			HitActor != nullptr)
		{
			LSLOG(Warning, TEXT("Hit Actor Name : %s"), *HitActor->GetName());

			FDamageEvent DamageEvent;
			HitActor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
		}
	}
}

void ALSCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	if (AssetLoaded != nullptr)
	{
		GetMesh()->SetSkeletalMesh(AssetLoaded);
	}
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

