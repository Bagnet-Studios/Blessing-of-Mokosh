// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HAGJ/Items/Weapons/BaseWeapon.h"

#include "WeaponComponent.generated.h"


class ABaseProjectile;
class ABaseCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAGJ_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UWeaponComponent();

	void Attack();
	void AttackRange();
	void DeSpawnWeapon() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponAttackSocketName = "WeaponSocket";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;	
	
private:
	UPROPERTY()
	ABaseWeapon* CurrentWeapon = nullptr;
	
	void SpawnWeapon();

private:
	ABaseCharacter* Character;
};
