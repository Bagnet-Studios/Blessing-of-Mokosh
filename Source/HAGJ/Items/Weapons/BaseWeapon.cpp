// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "HAGJ/Characters/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ABaseCharacter>(GetOwner());
	WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::BeginOverlap);
}

void ABaseWeapon::Attack()
{
	UE_LOG(LogTemp, Display, TEXT("FIRE!"));
}

void ABaseWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UGameplayStatics::ApplyDamage(OtherActor, Damage, Character->GetController(), Character, DamageType);
}
