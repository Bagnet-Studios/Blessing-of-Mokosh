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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundBase* ReviveSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundBase* BowSound = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void RotateCharacterToCursor();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency", meta = (ClampMin = 0.f, ClampMax = 999.f, UIMin = 0.f, UIMax = 999.f))
	int32 ArrowCount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency", meta = (ClampMin = 0.f, ClampMax = 999.f, UIMin = 0.f, UIMax = 999.f))
	int32 LivingWater = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency", meta = (ClampMin = 0.f, ClampMax = 999.f, UIMin = 0.f, UIMax = 999.f))
	int32 Pie = 0.f;

protected:
	virtual void BeginPlay() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;
	
	FTimerHandle DeathTimer;

	virtual void OnDeath();
	
private:	
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void Turn(float Value);
	void TurnRate(float Value);
	void OnHealthChanged(float Health);
	void DestroyCharacter();
	
};


