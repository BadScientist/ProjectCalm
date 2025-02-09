// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "QuestObjective.generated.h"

class AInteractableActor;
class USphereComponent;
class UItemData;
enum EObjectiveType : uint8;
struct FPhotoData;
struct FObjectiveDetails;
struct FSubjectBehaviorPair;


UCLASS()
class PROJECTCALM_API AQuestObjective : public AActor
{
	GENERATED_BODY()

protected:
	TEnumAsByte<EObjectiveType> ObjectiveType;

	uint32 QuestID{0x00000000};
	uint32 StageIdx{0};  // The index of the FStageDetails within the FQuestDetails::Stages array
	uint32 ObjectiveIdx{0};  // The index of the FObjectiveDetails this actor represents within the FStageDetails::Objectives array
    bool bIsComplete{false};
    bool bIsOptional{false};
	
public:
	// Sets default values for this actor's properties
	AQuestObjective();

	uint32 GetQuestID() {return QuestID;}
	uint32 GetStageIdx() {return StageIdx;}
	uint32 GetObjectiveIdx() {return ObjectiveIdx;}

	virtual void Setup(FObjectiveDetails ObjectiveDetails, uint32 InQuestID, uint32 InStageIdx, uint32 InObjectiveIdx);
	virtual void Teardown();
	
	virtual bool IsComplete() {return bIsComplete;}

protected:
	virtual void CompleteObjective();

};


UCLASS()
class PROJECTCALM_API ATravelObjective : public AQuestObjective
{
	GENERATED_BODY()

    USphereComponent* SphereCollision;
	
public:
	// Sets default values for this actor's properties
	ATravelObjective();

	virtual void Setup(FObjectiveDetails ObjectiveDetails, uint32 InQuestID, uint32 InStageIdx, uint32 InObjectiveIdx) override;

	UFUNCTION()
	virtual void OnTriggerVolumeEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	virtual void BeginPlay() override;

};


UCLASS()
class PROJECTCALM_API AInteractObjective : public AQuestObjective
{
	GENERATED_BODY()	

protected:
	TSubclassOf<AInteractableActor> InteractTargetClass;
	AInteractableActor* InteractTarget;
	
public:
	// Sets default values for this actor's properties
	AInteractObjective();

	virtual void Setup(FObjectiveDetails ObjectiveDetails, uint32 InQuestID, uint32 InStageIdx, uint32 InObjectiveIdx) override;

	UFUNCTION()
	virtual void OnInteract(AInteractableActor* Interactable);

protected:
	virtual void BeginPlay() override;

};


UCLASS()
class PROJECTCALM_API APhotoObjective : public AInteractObjective
{
	GENERATED_BODY()

    TArray<FSubjectBehaviorPair> PhotoTargets;
	float Score{0};

	bool bHasQualifiedPhoto{false};
	
public:
	// Sets default values for this actor's properties
	APhotoObjective();

	virtual void Setup(FObjectiveDetails ObjectiveDetails, uint32 InQuestID, uint32 InStageIdx, uint32 InObjectiveIdx) override;

	UFUNCTION()
	virtual void CheckNewPhoto(FPhotoData Photo);

	UFUNCTION()
	virtual void CheckAllPhotos();

	virtual void OnInteract(AInteractableActor* Interactable) override;

protected:
	virtual void BeginPlay() override;

private:
	bool IsQualifiedPhoto(FPhotoData Photo);

};
