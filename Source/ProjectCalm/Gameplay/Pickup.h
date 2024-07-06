// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"

#include "Pickup.generated.h"

class APlayerCharacter;
class UItemData;
class UStaticMeshComponent;
class UBoxComponent;


UCLASS()
class PROJECTCALM_API APickup : public AInteractableActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Contents)
	TArray<UItemData*> Items;

	UPROPERTY(EditDefaultsOnly, Category = Spawn)
	float Duration{1.0f};
	UPROPERTY(EditDefaultsOnly, Category = Spawn)
	float BounceHeight{15.0f};
	UPROPERTY(EditDefaultsOnly, Category = Spawn)
	float StartScale{0.1f};

	float SpawnTime{0.0f};
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void AbortPickup(APlayerCharacter* InteractingPlayer);

public:
	// START INTERACTABLEINTERFACE
    virtual void Interact(APlayerCharacter* InteractingPlayer) override;
	// END INTERACTABLEINTERFACE

	void Setup(UItemData* ItemData);

};
