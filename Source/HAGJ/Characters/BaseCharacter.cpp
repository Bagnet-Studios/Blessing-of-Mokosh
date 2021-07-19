#include "HAGJ/Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HAGJ/Components/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Projectile point"));
	ProjectileSpawnPoint->SetupAttachment(GetRootComponent());
	
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

void ABaseCharacter::SetIsAttacking(bool IsAttacking)
{
	bIsAttacking = IsAttacking;
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

// float ABaseCharacter::GetMovementDirection() const
// {
// 	if(GetVelocity().IsZero())
// 	{
// 		return 0.f;
// 	}
// 	const auto VelocityNormal = GetVelocity().GetSafeNormal();
// 	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
// 	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
// 	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
// 	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
// }