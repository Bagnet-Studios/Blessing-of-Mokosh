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
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWeaponComponent* WeaponComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UDecalComponent* CursorToWorld;

	UFUNCTION(BlueprintCallable)
	float GetMovementDirection() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	UFUNCTION(BlueprintImplementableEvent)
	void RotateCharacterToCursor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Currency")
	int32 ArrowCount = 10;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathAnimMontage = nullptr;
	
	FTimerHandle DeSpawnWeaponTimerHandle;
	
private:	
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void OnHealthChanged(float Health);
	void OnDeath();
	void DestroyCharacter();
	
};


