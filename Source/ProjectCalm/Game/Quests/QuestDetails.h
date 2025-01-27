#pragma once

#include "Engine/DataTable.h"

#include "ProjectCalm/Photos/PhotoSubjectData.h"
#include "ProjectCalm/AI/PhotoSubjectBehavior.h"
#include "QuestDetails.generated.h"

class AInteractableActor;
class UItemData;


// @todo: implement manual quest tracking
UENUM()
enum EObjectiveType
{
    OBJECTIVE_BASE  =0  UMETA(DisplayName = "Default"),
    TRAVEL          =1	UMETA(DisplayName = "Travel"),
	INTERACT        =2	UMETA(DisplayName = "Interact"),
	PHOTO           =3	UMETA(DisplayName = "Photo")
};


USTRUCT(BlueprintType)
struct FSubjectBehaviorPair
{
	GENERATED_BODY()

    FSubjectBehaviorPair(){};
    
    UPROPERTY(EditAnywhere)
    TEnumAsByte<ESubjectName> Subject{ESubjectName::NO_NAME};
    
    UPROPERTY(EditAnywhere)
    TEnumAsByte<EPhotoSubjectBehavior> Behavior{EPhotoSubjectBehavior::NONE};
};


USTRUCT(BlueprintType)
struct FObjectiveDetails
{
	GENERATED_BODY()

    FObjectiveDetails(){};
    
    UPROPERTY(EditAnywhere)
    TEnumAsByte<EObjectiveType> Type{EObjectiveType::OBJECTIVE_BASE};

    UPROPERTY(EditAnywhere)
    FString ObjectiveDescription{FString()};
    
    UPROPERTY(EditAnywhere)
    bool bIsOptional{false};

    bool bIsComplete{false};
    
    UPROPERTY(EditAnywhere)
    FVector TargetLocation{FVector::ZeroVector};
    
    UPROPERTY(EditAnywhere)
    FRotator TargetRotation{FRotator::ZeroRotator};

    // TRAVEL OBJECTIVE
    UPROPERTY(EditAnywhere)
    float Tolerance{300.0f};
    //END TRAVEL OBJECTIVE

    // INTERACT OBJECTIVE
    UPROPERTY(EditAnywhere)
    TSubclassOf<AInteractableActor> InteractTargetClass;

    UPROPERTY(EditAnywhere)
    bool bSpawnNewInteractable{false};
    //END INTERACT OBJECTIVE

    // PHOTO OBJECTIVE
    UPROPERTY(EditAnywhere)
    TArray<FSubjectBehaviorPair> PhotoTargets;

    UPROPERTY(EditAnywhere)
    float Score{0.0f};
    //END PHOTO OBJECTIVE
};


USTRUCT(BlueprintType)
struct FStageDetails
{
	GENERATED_BODY()

    FStageDetails(){};
    
    UPROPERTY(EditDefaultsOnly)
    TArray<FString> IntroDialogue;
    
    UPROPERTY(EditDefaultsOnly)
    TArray<FString> OngoingDialogue;
    
    UPROPERTY(EditDefaultsOnly)
    FString StageDescription{FString()};

    UPROPERTY(EditDefaultsOnly)
    TArray<FObjectiveDetails> Objectives;

    UPROPERTY(EditDefaultsOnly)
    TArray<uint32> EquipmentRewards;

    bool bIntroPlayed{false};
    bool bIsComplete{false};
};


USTRUCT(BlueprintType)
struct PROJECTCALM_API FQuestDetails : public FTableRowBase 
{
	GENERATED_BODY()

    FQuestDetails(){};
    
    UPROPERTY(EditDefaultsOnly)
    uint32 QuestID{0x00000000};
    
    UPROPERTY(EditDefaultsOnly)
    uint32 NextQuestID{0x00000000};
    
    UPROPERTY(EditDefaultsOnly)
    FName Name{FName()};
    
    UPROPERTY(EditDefaultsOnly)
    FString QuestDescription{FString()};
    
    UPROPERTY(EditDefaultsOnly)
    TArray<FStageDetails> Stages;
    
    UPROPERTY(EditDefaultsOnly)
    bool bIsHidden{false};

    UPROPERTY(EditDefaultsOnly)
    int32 ActiveStageIdx{-1};

    bool IsComplete() {return ActiveStageIdx >= Stages.Num();}
    bool IsActive() {return ActiveStageIdx >= 0 && !IsComplete();}

};

inline bool operator==(const FQuestDetails& lhs, const FQuestDetails& rhs) {return lhs.QuestID == rhs.QuestID;}
inline bool operator!=(const FQuestDetails& lhs, const FQuestDetails& rhs) {return lhs.QuestID != rhs.QuestID;}