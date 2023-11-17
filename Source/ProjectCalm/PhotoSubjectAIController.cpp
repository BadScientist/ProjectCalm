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

void APhotoSubjectAIController::DetermineReaction(EAlertLevel InAlertLevel, AActor* ReactionSource)
{
    if (AlertLevel > InAlertLevel) {return;}
    
    TSubclassOf<UPhotoSubjectDataComponent> DataComponentClass;
    UPhotoSubjectDataComponent* DataComponent = Cast<UPhotoSubjectDataComponent>(ReactionSource->GetComponentByClass(DataComponentClass));
    if (DataComponentClass != nullptr)
    {
        for (ESubjectName PredatorName : Predators)
        {
            if (DataComponent->GetSubjectName() == PredatorName)
            {
                SetAlertLevelKeyValue(BBKEY_ALERT_LEVEL, InAlertLevel);
                GetBlackboardComponent()->SetValueAsObject(TEXT("ReactionTarget"), ReactionSource);
                return;
            }
        }

        for (ESubjectName PreyName : Prey)
        {
            if (DataComponent->GetSubjectName() == PreyName)
            {
                SetAlertLevelKeyValue(BBKEY_ALERT_LEVEL, InAlertLevel == EAlertLevel::ALARMED ? EAlertLevel::AGGRO : InAlertLevel);
                return;
            }
        }
    }

    else if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ReactionSource))
    {
        SetAlertLevelKeyValue(BBKEY_ALERT_LEVEL, InAlertLevel);
        GetBlackboardComponent()->SetValueAsObject(TEXT("ReactionTarget"), ReactionSource);
        return;
    }
}
