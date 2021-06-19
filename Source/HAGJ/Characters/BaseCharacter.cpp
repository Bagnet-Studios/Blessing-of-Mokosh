// Fill out your copyright notice in the Description page of Project Settings.


#include <HAGJ/Characters/BaseCharacter.h>

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HAGJ/Components/WeaponComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("UpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ABaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ABaseCharacter::OnHealthChanged);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ABaseCharacter::GetMovementDirection() const
{
	if(GetVelocity().IsZero())
	{
		return FVector::ZeroVector;
	}
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct;
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAction("Attack", IE_Released, WeaponComponent, &UWeaponComponent::Attack);
}

void ABaseCharacter::MoveForward(float Amount)
{
	if (Amount == 0.f)
	{
		return;
	}
	AddMovementInput(CameraComponent->GetForwardVector(), Amount);
}

void ABaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.f)
	{
		return;
	}
	AddMovementInput(CameraComponent->GetRightVector(), Amount);
}

void ABaseCharacter::OnHealthChanged(float Health)
{
	UE_LOG(LogTemp, Warning, TEXT("Character %s Health: %f"), *GetOwner()->GetName(), HealthComponent->GetHealth());
}

void ABaseCharacter::OnDeath()
{
	PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();
}
