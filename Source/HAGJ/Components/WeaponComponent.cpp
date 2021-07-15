#include "WeaponComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "HAGJ/Characters/BaseCharacter.h"
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

	PlayerCharacter = Cast<ABaseCharacter>(GetOwner());
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

void UWeaponComponent::SpawnRangedWeapon()
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
	RangedCurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(RangedWeaponClass);
	if(!RangedCurrentWeapon)
	{
		return;
	}
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	RangedCurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "RangedWeaponSocket");
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
	if(!CurrentWeapon
		|| PlayerCharacter->HealthComponent->IsDead()
		|| bInputAttack == false
		|| !PlayerCharacter->MeleeAnimMontage)
	{
		return;
	}

	bInputAttack = false;
	PlayerCharacter->SetIsAttacking(true);
	
	RotateCharacterToCursor();
	
	float MeleeAttackAnimationDuration = PlayerCharacter->PlayAnimMontage(PlayerCharacter->MeleeAnimMontage);
	GetWorld()->GetTimerManager().SetTimer(AttackAnimTimer, this, &UWeaponComponent::CanAttack, 1.0f, false, MeleeAttackAnimationDuration);
}

void UWeaponComponent::CanAttack()
{
	bInputAttack = true;
	PlayerCharacter->SetIsAttacking(false);
}

void UWeaponComponent::AttackRange()
{
	if(PlayerCharacter->ArrowCount <= 0
		|| PlayerCharacter->HealthComponent->IsDead()
		|| bInputAttack == false
		|| !PlayerCharacter->RangeAnimMontage
		|| !PlayerCharacter->bCanShoot)
	{
		return;
	}
	
	PlayerCharacter->PlayAnimMontage(PlayerCharacter->RangeAnimMontage);
	bInputAttack = false;
	
	RotateCharacterToCursor();
	
	if(ProjectileClass)
	{
		FVector SpawnLocation = PlayerCharacter->ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = PlayerCharacter->ProjectileSpawnPoint->GetComponentRotation();
		CurrentProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		CurrentProjectile->SetOwner(PlayerCharacter);
		PlayerCharacter->ArrowCount--;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerCharacter->BowSound, PlayerCharacter->GetActorLocation());
	}
	GetWorld()->GetTimerManager().SetTimer(AttackAnimTimer, this, &UWeaponComponent::CanAttack, 1.0f, false, 1.f);
}

void UWeaponComponent::RotateCharacterToCursor()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	FRotator PlayerRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), HitResult.ImpactPoint);
	PlayerCharacter->SetActorRotation(FRotator(0.f, PlayerRotation.Yaw, 0.f));
}
