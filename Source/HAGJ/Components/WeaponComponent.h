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
	UFUNCTION(BlueprintCallable)
	void AttackRange();
	void DeSpawnWeapon() const;

	UPROPERTY(BlueprintReadWrite)
	ABaseWeapon* CurrentWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite)
	ABaseProjectile* CurrentProjectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageMultiplier = 1.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bCanAttack = false;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponAttackSocketName = "WeaponSocket";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;	
	
private:
	UFUNCTION()
	void StopAttack();
	
	void SpawnWeapon();

private:
	ABaseCharacter* PlayerCharacter;
};
