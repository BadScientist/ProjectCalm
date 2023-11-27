// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoSubjectAIController.h"
#include "PhotoSubjectDataComponent.h"
#include "PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

APhotoSubjectAIController::APhotoSubjectAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APhotoSubjectAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
}

void APhotoSubjectAIController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTree != nullptr) {RunBehaviorTree(BehaviorTree);}
}

void APhotoSubjectAIController::SetAlertLevelKeyValue(const FName& KeyName, EAlertLevel InAlertLevel)
{
    if (KeyName == BBKEY_ALERT_LEVEL)
    {
        if (InAlertLevel == AlertLevel)
        {
            LastBehavior = ActiveBehavior;
            SetBehaviorKeyValue(BBKEY_ACTIVE_BEHAVIOR, EPhotoSubjectBehavior::RESTART_LAST);
        }
        else {AlertLevel = InAlertLevel;}
    }
    GetBlackboardComponent()->SetValueAsEnum(KeyName, InAlertLevel);
}

void APhotoSubjectAIController::SetBehaviorKeyValue(const FName& KeyName, EPhotoSubjectBehavior InBehavior)
{
    EPhotoSubjectBehavior NewBehavior = InBehavior;
    if (KeyName == BBKEY_ACTIVE_BEHAVIOR)
    {
        if (ActiveBehavior == EPhotoSubjectBehavior::RESTART_LAST)
        {
            NewBehavior = LastBehavior;
            LastBehavior = EPhotoSubjectBehavior::NONE;
        }
        ActiveBehavior = NewBehavior;
    }
    GetBlackboardComponent()->SetValueAsEnum(KeyName, NewBehavior);
}

void APhotoSubjectAIController::SetVectorKeyValue(const FName& KeyName, FVector InVector)
{
    if (KeyName == BBKEY_HOME_LOCATION) {HomeLocation = InVector;}
    if (KeyName == BBKEY_TARGET_LOCATION) {TargetLocation = InVector;}
    GetBlackboardComponent()->SetValueAsVector(KeyName, InVector);
}

void APhotoSubjectAIController::SetBoolKeyValue(const FName &KeyName, bool bInValue)
{
    GetBlackboardComponent()->SetValueAsBool(KeyName, bInValue);
}

void APhotoSubjectAIController::SetObjectKeyValue(const FName &KeyName, UObject *InObject)
{
    GetBlackboardComponent()->SetValueAsObject(KeyName, InObject);
}

void APhotoSubjectAIController::DetermineReaction(EAlertLevel InAlertLevel, AActor* ReactionSource)
{
    if (AlertLevel > InAlertLevel) {return;}
    
    UPhotoSubjectDataComponent* DataComponent = ReactionSource->FindComponentByClass<UPhotoSubjectDataComponent>();
    EAlertLevel NewAlertLevel = InAlertLevel;

    if (DataComponent != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("AICONTROLLER::%s::ReactionSource SubjectName:%s"), *GetActorNameOrLabel(), *SubjectName::SubjectNameEnumToString(DataComponent->GetSubjectName()));
        for (ESubjectName PreyName : Prey)
        {
            if (DataComponent->GetSubjectName() == PreyName && NewAlertLevel == EAlertLevel::ALARMED)
            {
                NewAlertLevel = EAlertLevel::AGGRO;
                break;
            }
        }
    }

    else if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ReactionSource))
    {
        if (bIsHostileToPlayer && NewAlertLevel == EAlertLevel::ALARMED)
        {
            NewAlertLevel = EAlertLevel::AGGRO;
        }
    }

    SetAlertLevelKeyValue(BBKEY_ALERT_LEVEL, NewAlertLevel);
    SetObjectKeyValue(BBKEY_REACTION_TARGET, ReactionSource);
}
