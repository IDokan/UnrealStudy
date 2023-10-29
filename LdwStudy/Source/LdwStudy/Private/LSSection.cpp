// Fill out your copyright notice in the Description page of Project Settings.


#include "LSSection.h"
#include "LSCharacter.h"
#include "LSItemBox.h"
#include "LSPlayerController.h"
#include "LSGameMode.h"

// Sets default values
ALSSection::ALSSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	FString AssetPath = TEXT("/Script/Engine.StaticMesh'/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else
	{
		LSLOG(Error, TEXT("Failed to load static mesh asset. : %s"), *AssetPath);
	}

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.f, 775.f, 300.f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	Trigger->SetCollisionProfileName(TEXT("LSTrigger"));

	FString GateAssetPath = TEXT("/Script/Engine.StaticMesh'/Game/Book/StaticMesh/SM_GATE.SM_GATE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		LSLOG(Error, TEXT("Failed to load static mesh asset. : %s"), *GateAssetPath);
	}

	static FName GateSockets[] = { {TEXT("+XGate")}, { TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	for (FName GateSocket : GateSockets)
	{
		LSCHECK(Mesh->DoesSocketExist(GateSocket));

		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());

		NewGate->SetStaticMesh(SM_GATE.Object);
		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation(FVector(0.f, -80.5f, 0.f));
		GateMeshes.Add(NewGate);

		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
		NewGateTrigger->SetBoxExtent(FVector(100.f, 100.f, 300.f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.f, 0.f, 250.f));
		NewGateTrigger->SetCollisionProfileName(TEXT("LSTrigger"));
		GateTriggers.Add(NewGateTrigger);
	}

	bNoBattle = false;

	EnemySpawnTime = 2.f;
	ItemBoxSpawnTime = 5.f;
}

void ALSSection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called when the game starts or when spawned
void ALSSection::BeginPlay()
{
	Super::BeginPlay();
	
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ALSSection::OnTriggerBeginOverlap);

	static FName GateSockets[] = { {TEXT("+XGate")}, { TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	for (uint32 i = 0; i < 4; ++i)
	{
		GateTriggers[i]->OnComponentBeginOverlap.AddDynamic(this, &ALSSection::OnGateTriggerBeginOverlap);
		GateTriggers[i]->ComponentTags.Add(GateSockets[i]);
	}
}

void ALSSection::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ALSSection::ESectionState::READY:
		Trigger->SetCollisionProfileName(TEXT("LSTrigger"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}

		OperateGates(true);
		break;
	case ALSSection::ESectionState::BATTLE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}

		OperateGates(false);

		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle, FTimerDelegate::CreateUObject(this, &ALSSection::OnNPCSpawn), EnemySpawnTime, false);
		GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			FVector2D RandXY = FMath::RandPointInCircle(600.f);
			GetWorld()->SpawnActor<ALSItemBox>(GetActorLocation() + FVector(RandXY, 30.f), FRotator::ZeroRotator);
		}), ItemBoxSpawnTime, false);
		break;
	case ALSSection::ESectionState::COMPLETE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("LSTrigger"));
		}

		OperateGates(true);
		break;
	default:
		break;
	}

	CurrentState = NewState;
	LSLOG(Warning, TEXT("CurrentState : %d"), StaticCast<int32>(CurrentState));
}

void ALSSection::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.f, -90.f, 0.f) : FRotator::ZeroRotator);
	}
}

void ALSSection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentState == ESectionState::READY)
	{
		SetState(ESectionState::BATTLE);
	}
}

void ALSSection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LSLOG(Warning, TEXT("%d"), OverlappedComponent->ComponentTags.Num());
	LSCHECK(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	if (!Mesh->DoesSocketExist(SocketName))
	{
		return;
	}

	// Depends on game progressions, there are section actors created already.
	// Skip instantiating a new section if it is.
	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllDynamicObjects);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(775.f),
		CollisionQueryParam
	);

	if (!bResult)
	{
		auto NewSection = GetWorld()->SpawnActor<ALSSection>(NewLocation, FRotator::ZeroRotator);
	}
	else
	{
		LSLOG(Warning, TEXT("New section area is not empty"));
	}
}

void ALSSection::OnNPCSpawn()
{
	GetWorld()->SpawnActor<ALSCharacter>(GetActorLocation() + FVector::UpVector * 88.f, FRotator::ZeroRotator);
	auto KeyNPC = GetWorld()->SpawnActor<ALSCharacter>(GetActorLocation() + FVector::UpVector * 88.f, FRotator::ZeroRotator);
	if (KeyNPC != nullptr)
	{
		KeyNPC->OnDestroyed.AddDynamic(this, &ALSSection::OnKeyNPCDestroyed);
	}
}

void ALSSection::OnKeyNPCDestroyed(AActor* DestroyedActor)
{
	auto LSCharacter = Cast<ALSCharacter>(DestroyedActor);
	LSCHECK(LSCharacter != nullptr);

	auto LSPlayerController = Cast<ALSPlayerController>(LSCharacter->LastHitBy);
	LSCHECK(LSPlayerController != nullptr);

	auto LSGameMode = Cast<ALSGameMode>(GetWorld()->GetAuthGameMode());
	LSCHECK(LSGameMode != nullptr);
	LSGameMode->AddScore(LSPlayerController);

	SetState(ESectionState::COMPLETE);
}