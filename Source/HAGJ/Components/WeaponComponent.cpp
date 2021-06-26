// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "HAGJ/Characters/BaseCharacter.h"
#include "HAGJ/GameModes/BaseGameMode.h"
#include "HAGJ/Items/Projectiles/BaseProjectile.h"
#include "HAGJ/Items/Weapons/BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();

	PlayerCharacter = Cast<ABaseCharacter>(GetOwner());
	//Character = Cast<ABaseCharacter>(GetOwner());
}

void UWeaponComponent::SpawnWeapon()
{
	if(!GetWorld())
	{
		return;
	}		
	ACharacter* Character = Cast<ACharacter>(GetOwner());
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
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "WeaponSocket");
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
	if(!CurrentWeapon || PlayerCharacter->HealthComponent->IsDead() || bCanAttack == true || bInputAttack == false)
	{
		return;
	}
	if(!PlayerCharacter->MeleeAnimMontage)
	{
		return;
	}
	PlayerCharacter->PlayAnimMontage(PlayerCharacter->MeleeAnimMontage);
	bInputAttack = false;
	GetWorld()->GetTimerManager().SetTimer(AttackAnimTimer, this, &UWeaponComponent::CanAttack, 1.0f, false, 1.2f);
}

void UWeaponComponent::CanAttack()
{
	bInputAttack = true;
}

void UWeaponComponent::AttackRange()
{
	if(PlayerCharacter->ArrowCount <= 0 || PlayerCharacter->HealthComponent->IsDead())
	{
		return;
	}
	if(!PlayerCharacter->RangeAnimMontage)
	{
		return;
	}	

	PlayerCharacter->PlayAnimMontage(PlayerCharacter->RangeAnimMontage);
	PlayerCharacter->RotateCharacterToCursor();
	if(ProjectileClass)
	{
		FVector SpawnLocation = PlayerCharacter->ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = PlayerCharacter->ProjectileSpawnPoint->GetComponentRotation();
		CurrentProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		CurrentProjectile->SetOwner(PlayerCharacter);
		PlayerCharacter->ArrowCount--;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerCharacter->BowSound, PlayerCharacter->GetActorLocation());
	}
}


