// Fill out your copyright notice in the Description page of Project Settings.


#include "LSItemBox.h"
#include "LSCharacter.h"
#include "LSWeapon.h"

// Sets default values
ALSItemBox::ALSItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.f, 42.f, 30.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Script/Engine.StaticMesh'/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}

	Box->SetRelativeLocation(FVector(0.f, -3.5f, -30.f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = ALSWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ALSItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALSItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ALSItemBox::OnCharacterOverlap);
}

// Called every frame
void ALSItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALSItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LSLOG_S(Warning);

	auto LSCharacter = Cast<ALSCharacter>(OtherActor);
	LSCHECK(LSCharacter != nullptr);

	if (LSCharacter != nullptr && WeaponItemClass != nullptr)
	{
		if (LSCharacter->CanSetWeapon())
		{
			// Equip a weapon
			auto NewWeapon = GetWorld()->SpawnActor<ALSWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			LSCharacter->SetWeapon(NewWeapon);

			// Play a particle
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &ALSItemBox::OnEffectFinished);
		}
		else
		{
			LSLOG(Warning, TEXT("%s can't equip weapon currently."), *LSCharacter->GetName());
		}
	}
}

void ALSItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}

