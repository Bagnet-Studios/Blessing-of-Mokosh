// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();	
}

void ABaseWeapon::Attack()
{
	UE_LOG(LogTemp, Display, TEXT("FIRE!"));
}
