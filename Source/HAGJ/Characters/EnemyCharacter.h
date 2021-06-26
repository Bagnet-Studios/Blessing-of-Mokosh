// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Components/SphereComponent.h"

#include "EnemyCharacter.generated.h"


UCLASS()
class HAGJ_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* AttackCollision;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bIsRanged = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float FireRange = 10000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float FireRate = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<ABaseProjectile> ProjectileClass;	

	void Tick(float DeltaTime);
	void BeginPlay();
	UFUNCTION(BlueprintCallable)
	void RotateTurret(FVector LookAtTarget);
	void CheckFireCondition();
	float ReturnDistanceToPlayer();
	void AttackRange();
	virtual void OnDeath() override;

	UFUNCTION(BlueprintImplementableEvent)
	void AddMoney();

	ABaseCharacter* PlayerPawn = nullptr;
	FTimerHandle FireRateTimerHandle;
};
