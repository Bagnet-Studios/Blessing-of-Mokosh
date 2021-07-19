// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HAGJ/Components/HealthComponent.h"
#include "HAGJ/Components/WeaponComponent.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
UCLASS()
class HAGJ_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWeaponComponent* WeaponComponent;

	// UFUNCTION(BlueprintCallable)
	// float GetMovementDirection() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathAnimMontage = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* MeleeAnimMontage = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* RangeAnimMontage = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	USoundBase* AttackSoundWave = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanShoot = true;
	
	void SetIsAttacking(bool IsAttacking);

protected:
	virtual void BeginPlay() override;
	
	FTimerHandle DeathTimer;
	virtual void OnDeath();
	
	//true = character is attacking and can't move
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAttacking = false;	
	
private:
	void OnHealthChanged(float Health);
	void DestroyCharacter();
};