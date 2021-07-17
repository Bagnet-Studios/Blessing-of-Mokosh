// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class HAGJ_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundBase* ReviveSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundBase* BowSound = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void RotateCharacterToCursor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency", meta = (ClampMin = 0.f, ClampMax = 999.f, UIMin = 0.f, UIMax = 999.f))
	int32 ArrowCount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency", meta = (ClampMin = 0.f, ClampMax = 999.f, UIMin = 0.f, UIMax = 999.f))
	int32 LivingWater = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency", meta = (ClampMin = 0.f, ClampMax = 999.f, UIMin = 0.f, UIMax = 999.f))
	int32 Pie = 0.f;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;
};
