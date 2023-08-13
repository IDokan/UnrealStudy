// Fill out your copyright notice in the Description page of Project Settings.


#include "LSPawn.h"

// Sets default values
ALSPawn::ALSPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	InputMapping = CreateDefaultSubobject<UInputMappingContext>(TEXT("INPUTMAPPING"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.f);
	Capsule->SetCapsuleRadius(34.f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_CARDBOARD.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// Animation Settings
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Script/Engine.AnimBlueprint'/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C'"));
	if (WARRIOR_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(WARRIOR_ANIM.Class);
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
}

// Called when the game starts or when spawned
void ALSPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(inputUpDown, ETriggerEvent::Triggered, this, &ALSPawn::UpDown);
	PEI->BindAction(inputLeftRight, ETriggerEvent::Triggered, this, &ALSPawn::LeftRight);
}

void ALSPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LSLOG_S(Warning);
}

void ALSPawn::PossessedBy(AController* NewController)
{
	LSLOG_S(Warning);
	Super::PossessedBy(NewController);
}

void ALSPawn::UpDown(const FInputActionValue& NewAxisValue)
{
	LSLOG(Warning, TEXT("%f"), static_cast<float>(NewAxisValue.Get<float>()));

	AddMovementInput(GetActorForwardVector(), NewAxisValue.Get<float>());
}

void ALSPawn::LeftRight(const FInputActionValue& NewAxisValue)
{
	LSLOG(Warning, TEXT("%f"), static_cast<float>(NewAxisValue.Get<float>()));

	AddMovementInput(GetActorRightVector(), NewAxisValue.Get<float>());
}