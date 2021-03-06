#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HAGJ/Items/Weapons/BaseWeapon.h"
#include "WeaponComponent.generated.h"


class ABaseProjectile;
class APlayerCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAGJ_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UWeaponComponent();

	void Attack();
	UFUNCTION(BlueprintCallable)
	void AttackRange();
	UFUNCTION(BlueprintCallable)
	void RotateCharacterToCursor();
	
	UFUNCTION(BlueprintCallable)
	void SpawnWeapon();
	UFUNCTION(BlueprintCallable)
	void SpawnRangedWeapon();
	void DeSpawnWeapon() const;

	UPROPERTY(BlueprintReadWrite)
	ABaseWeapon* CurrentWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite)
	ABaseWeapon* RangedCurrentWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite)
	ABaseProjectile* CurrentProjectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageMultiplier = 1.f;

	//Allow make damage
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bCanAttack = false;
	
	FTimerHandle AttackAnimTimer;
	UFUNCTION()
	void CanAttack();
	//Allow make attack/attack animation
	bool bInputAttack = true;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponAttackSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABaseWeapon> RangedWeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName RangedWeaponAttackSocketName = "RangedWeaponSocket";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;	
	
private:
	APlayerCharacter* PlayerCharacter;
};
