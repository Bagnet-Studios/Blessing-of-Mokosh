// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "HAGJ/Items/Projectiles/BaseProjectile.h"
#include "Kismet/GameplayStatics.h"


AEnemyCharacter::AEnemyCharacter()
{
	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Collision"));
	AttackCollision->SetupAttachment(GetRootComponent());
	AttackCollision->SetSphereRadius(100.f);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!PlayerPawn || ReturnDistanceToPlayer() > FireRange)
	{
		return;
	}
	RotateTurret(PlayerPawn->GetActorLocation());
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(bIsRanged)
	{
		GetWorld()->GetTimerManager().SetTimer(
			FireRateTimerHandle, this, &AEnemyCharacter::CheckFireCondition, FireRate, true);	
	}	

	PlayerPawn = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AEnemyCharacter::RotateTurret(FVector LookAtTarget)
{
	FVector LookAtTargetClean = FVector(LookAtTarget.X, LookAtTarget.Y, GetMesh()->GetComponentLocation().Z);
	FVector StartLocation = GetMesh()->GetComponentLocation();
	FRotator TurretRotation = FVector(LookAtTargetClean - StartLocation).Rotation();
	TurretRotation.Yaw -= 90.f;
	GetMesh()->SetWorldRotation(TurretRotation);	
}

void AEnemyCharacter::CheckFireCondition()
{
	if(!PlayerPawn || PlayerPawn->HealthComponent->IsDead())
	{
		return;
	}
	if(ReturnDistanceToPlayer() <= FireRange)
	{
		AttackRange();
	}
}

float AEnemyCharacter::ReturnDistanceToPlayer()
{
	if(!PlayerPawn)
	{
		return 10000000.f;
	}
	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void AEnemyCharacter::AttackRange()
{
	if(ProjectileClass)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		ABaseProjectile* TempProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

void AEnemyCharacter::OnDeath()
{
	Super::OnDeath();
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

