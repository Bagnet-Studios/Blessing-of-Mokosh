// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "HAGJ/Characters/BaseCharacter.h"
#include "HAGJ/GameModes/BaseGameMode.h"
#include "HAGJ/Items/Projectiles/BaseProjectile.h"
#include "HAGJ/Items/Weapons/BaseWeapon.h"
#include "Kismet/KismetMathLibrary.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();

	Character = Cast<ABaseCharacter>(GetOwner());
}

void UWeaponComponent::SpawnWeapon()
{
	if(!GetWorld())
	{
		return;
	}		
	//Character = Cast<ACharacter>(GetOwner());
	if(!Character)
	{
		return;
	}
	CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
	if(!CurrentWeapon)
	{
		return;
	}
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttackSocketName);
}

void UWeaponComponent::DeSpawnWeapon() const
{
	if(!GetWorld())
	{
		return;
	}

	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->OnHolderDeath();
}

void UWeaponComponent::Attack()
{
	Character->RotateCharacterToCursor();
	if(!CurrentWeapon)
	{
		return;
	}
	CurrentWeapon->Attack();
}

void UWeaponComponent::AttackRange()
{
	Character->RotateCharacterToCursor();
	if(ProjectileClass)
	{
		FVector SpawnLocation = Character->ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = Character->ProjectileSpawnPoint->GetComponentRotation();
		ABaseProjectile* TempProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(Character);
	}
}


