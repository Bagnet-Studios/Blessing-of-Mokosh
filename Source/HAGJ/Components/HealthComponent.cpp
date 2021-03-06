// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "HAGJ/Characters/BaseCharacter.h"
#include "HAGJ/Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	AActor* ComponentOwner = GetOwner();
	if(ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
	}
}

void UHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Character %s Health: %f"), *GetOwner()->GetName(), Health);
	
	if(Damage <= 0.f || IsDead() || !GetWorld())
	{
		return;
	}

	SetHealth(Health - Damage);
	
	if(IsDead()) 
	{
		APlayerCharacter* Character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if(DamagedActor == Character)
		{
			if(Character->LivingWater == 0.f)
			{
				OnDeath.Broadcast();	
			}
			else
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Character->ReviveSound,  Character->GetActorLocation());
				Character->LivingWater--;
				Health = MaxHealth;
			}
		}
		else if(DamagedActor != Character)
		{
			OnDeath.Broadcast();
		}
	}
}


