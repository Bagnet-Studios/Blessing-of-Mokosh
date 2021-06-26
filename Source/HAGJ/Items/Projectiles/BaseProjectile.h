// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class ABaseCharacter;
class UProjectileMovementComponent;
UCLASS()
class HAGJ_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	
	ABaseCharacter* Character = nullptr;
	
protected:
	virtual void BeginPlay() override;

private:
	//Components
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement settings", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
    UPROPERTY(EditDefaultsOnly, Category = "Movement settings", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;
	
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 0, DefaultMin = 0, AllowPrivateAccess = "true"))
	float MovementSpeed = 1300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = 0, DefaultMin = 0, AllowPrivateAccess = "true"))
	float Damage = 50.f;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = 0, DefaultMin = 0, AllowPrivateAccess = "true"))
	// float DamageMultiplier = 1.0f;
	
	//Functions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse, const FHitResult& Hit);
};
