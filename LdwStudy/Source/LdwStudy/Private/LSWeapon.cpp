// Fill out your copyright notice in the Description page of Project Settings.


#include "LSWeapon.h"

// Sets default values
ALSWeapon::ALSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight'"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	AttackRange = 150.f;
	AttackDamageMin = -2.f;
	AttackDamageMax = 10.f;
	AttackModifierMin = 0.85f;
	AttackModifierMax = 1.25f;
}

float ALSWeapon::GetAttackRange() const
{
	return AttackRange;
}

float ALSWeapon::GetAttackDamage() const
{
	return AttackDamage;
}

float ALSWeapon::GetAttackModifier() const
{
	return AttackModifier;
}

// Called when the game starts or when spawned
void ALSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AttackDamage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	AttackModifier = FMath::RandRange(AttackModifierMin, AttackModifierMax);
	LSLOG(Warning, TEXT("Weapon Damage : %f, Modifier : %f"), AttackDamage, AttackModifier);
}

// Called every frame
void ALSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

