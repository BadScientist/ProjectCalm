// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPerceptionComponent.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

UPCPerceptionComponent::UPCPerceptionComponent(const FObjectInitializer& ObjectInitializer) : 
    UAIPerceptionComponent(ObjectInitializer)
{
    bAutoActivate = true;
    
    TObjectPtr<UAISenseConfig_Hearing> HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));    
    TObjectPtr<UAISenseConfig_Sight> SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    CHECK_NULLPTR_RET(HearingConfig, LogAIPerception, "PCPerceptionComponent:: Failed to create SenseConfig_Hearing");
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    HearingConfig->SetMaxAge(5.0f);
    ConfigureSense(*HearingConfig);

    CHECK_NULLPTR_RET(SightConfig, LogAIPerception, "PCPerceptionComponent:: Failed to create SenseConfig_Sight");
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->SetMaxAge(5.0f);
    SightConfig->PeripheralVisionAngleDegrees = 70;
    SightConfig->PointOfViewBackwardOffset = 100.0f;
    SightConfig->NearClippingRadius = 100.0f;
    ConfigureSense(*SightConfig);
}
