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

	Character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnHit);
}

void ABaseWeapon::Attack()
{
	UE_LOG(LogTemp, Display, TEXT("FIRE!"));
	//Play anim montage
	bIsAttacking = true;
	//On end animation -> bool bAttack = false
}

void ABaseWeapon::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bIsAttacking == false)
	{
		return;
	}
		
	if(!Character)
	{
		return;
	}
	
	ABaseCharacter* DamagedActor = Cast<ABaseCharacter>(OtherActor);
	UGameplayStatics::ApplyDamage(DamagedActor, Damage, Character->GetInstigatorController(), this, DamageType);	
}
