// Fill out your copyright notice in the Description page of Project Settings.


#include <HAGJ/Characters/BaseCharacter.h>

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HAGJ/Components/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("UpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Projectile point"));
	ProjectileSpawnPoint->SetupAttachment(GetRootComponent());
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
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
	//UE_LOG(LogTemp, Warning, TEXT("%s"), WeaponComponent->bCanAttack ? TEXT("1") : TEXT("0"));
}

float ABaseCharacter::GetMovementDirection() const
{
	if(GetVelocity().IsZero())
	{
		return 0.f;
	}
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, WeaponComponent, &UWeaponComponent::Attack);
	PlayerInputComponent->BindAction("AttackRange", IE_Pressed, WeaponComponent, &UWeaponComponent::AttackRange);
}

void ABaseCharacter::MoveForward(float Amount)
{
	if ((Controller != nullptr) && (Amount != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = CameraComponent->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Amount);
	}
}

void ABaseCharacter::MoveRight(float Amount)
{
	if ( (Controller != nullptr) && (Amount != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = CameraComponent->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Amount);
	}
}

void ABaseCharacter::OnHealthChanged(float Health)
{
	//UE_LOG(LogTemp, Warning, TEXT("Character %s Health: %f"), *GetOwner()->GetName(), HealthComponent->GetHealth());
}

void ABaseCharacter::OnDeath()
{	
	if(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		GetCharacterMovement()->DisableMovement();
		GetController()->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PrimaryActorTick.bCanEverTick = false;

	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ABaseCharacter::DestroyCharacter, 0.01f, false, 3.f);	
}

void ABaseCharacter::DestroyCharacter()
{
	Destroy();
	WeaponComponent->DeSpawnWeapon();
	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
}
