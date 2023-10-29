// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoSubjectAIController.h"

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

    SetAlertLevel(EAlertLevel::CALM);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (PlayerPawn != nullptr) 
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    }
}

void APhotoSubjectAIController::SetAlertLevel(EAlertLevel InAlertLevel)
{
    AlertLevel = InAlertLevel;
    GetBlackboardComponent()->SetValueAsEnum(TEXT("AlertLevel"), InAlertLevel);
}

void APhotoSubjectAIController::SetActiveBehavior(EPhotoSubjectBehavior InBehavior)
{
    ActiveBehavior = InBehavior;
    GetBlackboardComponent()->SetValueAsEnum(TEXT("ActiveBehavior"), InBehavior);
}

void APhotoSubjectAIController::SetHomeLocation(FVector InLocation)
{
    HomeLocation = InLocation;
    GetBlackboardComponent()->SetValueAsVector(TEXT("HomeLocation"), InLocation);
}
