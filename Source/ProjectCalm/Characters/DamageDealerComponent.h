// Copyright 2024 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageDealerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UDamageDealerComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float DefaultDamage{10.0f};

	AActor* TargetActor{nullptr};

public:
	UDamageDealerComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetTarget(AActor* InTargetActor) {TargetActor = InTargetActor;}
	void DealDamage(float Damage = -1.0f, AActor* InTargetActor = nullptr, FString DamageMessage = FString());

		
};
