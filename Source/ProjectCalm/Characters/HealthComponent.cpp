// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ProjectCalm/AI/PhotoSubjectAIController.h"

#include "GameFramework/Character.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (IsDead() || FMath::IsNearlyEqual(CurrentHealth, MaxHealth)) {return;}

	UWorld* World = GetWorld();
	float CurrentTime = World == nullptr ? 0.0f : World->GetTimeSeconds();
	if (CurrentTime - LastDamageTime >= RegenDelay)
	{
		CurrentHealth = FMath::Min(CurrentHealth + RegenRate * DeltaTime, MaxHealth);
	}
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	if (IsDead()) {Die();}
	
}

void UHealthComponent::Die()
{
	if (OnDeath.IsBound()) {OnDeath.Broadcast(LastDamageMessage);}
}

bool UHealthComponent::IsDead() const
{
	return CurrentHealth < 0.001;
}

float UHealthComponent::ReceiveDamage(float Damage, UObject *DamageSource, FString DamageMessage)
{
	if (IsDead()) {return 0.0f;}

	float ActualDamage = (Damage > CurrentHealth) ? CurrentHealth : Damage;
	if (ActualDamage > 0)
	{
		UWorld* World = GetWorld();
		float CurrentTime = World == nullptr ? 0.0f : World->GetTimeSeconds();

		LastDamageTime = CurrentTime;
		LastDamageMessage = DamageMessage;
		if (OnDamageTaken.IsBound()) {OnDamageTaken.Broadcast();}
	}
	
	CurrentHealth -= ActualDamage;
	if (IsDead()) {Die();}
    
	return ActualDamage;
}

void UHealthComponent::Reset()
{
	CurrentHealth = MaxHealth;
}
