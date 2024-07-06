// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "ProjectCalm/Utilities/WidgetVector.h"

#include "QuestGiver.generated.h"

class UArrowComponent;
class APlayerCharacter;
class UTexture2D;
class APickup;
class UItemData;
struct FQuestDetails;


UCLASS()
class PROJECTCALM_API AQuestGiver : public AInteractableActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reward, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RewardSpawnPoint{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reward, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* Arrow{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = BasicInfo, meta = (AllowPrivateAccess = "true"))
    FName DisplayName{"DefaultDisplayName"};

    UPROPERTY(EditDefaultsOnly, Category = BasicInfo, meta = (AllowPrivateAccess = "true"))
    UTexture2D* Portrait{nullptr};

    UPROPERTY(EditAnywhere, Category = BasicInfo, meta = (AllowPrivateAccess = "true"))
    TArray<FString> DefaultDialogue;

    UPROPERTY(EditAnywhere, Category = Quest, meta = (AllowPrivateAccess = "true"))
    uint32 CurrentQuestID{0};

    uint32 CurrentStageIdx{0};

	
public:	
	// Sets default values for this actor's properties
    AQuestGiver();
	AQuestGiver(const FObjectInitializer& ObjectInitializer) : AQuestGiver(){};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    void HandleInteraction();
    uint32 SpawnRewards(TArray<uint32> ItemIDs);

public:
	// START INTERACTABLEINTERFACE
    virtual void Interact(APlayerCharacter* InteractingPlayer) override;
	// END INTERACTABLEINTERFACE

    UFUNCTION()
    void OnQuestUpdated(FQuestDetails QuestDetails);

};
