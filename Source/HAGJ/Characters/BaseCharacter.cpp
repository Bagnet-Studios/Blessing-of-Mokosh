// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

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
	
	GetController()->SetControlRotation(FRotator(0, 0 ,CameraComponent->GetComponentRotation().Yaw));
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ABaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.f)
	{
		return;
	}
	AddMovementInput(GetActorRightVector(), Amount);
}

void ABaseCharacter::OnHealthChanged(float Health)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Health: %f"), HealthComponent->GetHealth());
}

void ABaseCharacter::OnDeath()
{
	PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();
}
