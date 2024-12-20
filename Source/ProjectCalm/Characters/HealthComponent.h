// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UDamageDealerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTakenDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathDelegate, FString, DeathMessage);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Health)
	float MaxHealth{100.0f};

	UPROPERTY(EditAnywhere, Category = Health, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RegenRate{15.0f};

	UPROPERTY(EditAnywhere, Category = Health, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RegenDelay{8.0f};

	float CurrentHealth{100.0f};
	float LastDamageTime{0.0f};

	FString LastDamageMessage;

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnDamageTakenDelegate OnDamageTaken;
	FOnDeathDelegate OnDeath;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Die();

public:
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const {return MaxHealth;}
	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const {return CurrentHealth;}

	bool IsDead() const {return FMath::IsNearlyZero(CurrentHealth);}

	float ReceiveDamage(float Damage, UObject* DamageSource = nullptr, FString DamageMessage = FString());

	void Reset();
		
};
