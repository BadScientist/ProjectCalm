// Copyright 2024 Joseph D Tong aka "BadScientist"


#include "DamageDealerComponent.h"
#include "HealthComponent.h"


UDamageDealerComponent::UDamageDealerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UDamageDealerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDamageDealerComponent::DealDamage(float Damage, AActor* InTargetActor, FString DamageMessage)
{
	if (InTargetActor != nullptr) {TargetActor = InTargetActor;}
	if (TargetActor == nullptr)
	{
		UE_LOG(LogDamage, Warning, TEXT("DamageDealerComponent:: DealDamage called without a target set!"));
		return;
	}

	if (Damage < 0) {Damage = DefaultDamage;}

	if (UHealthComponent* TargetHealth = TargetActor->FindComponentByClass<UHealthComponent>())
	{
		TargetHealth->ReceiveDamage(Damage, GetOwner(), DamageMessage);
	}

}
