// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoSubjectAIController.h"
#include "PCPerceptionComponent.h"
#include "ProjectCalm/Photos/PhotoSubjectDataComponent.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Photos/PhotoSubjectName.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"


APhotoSubjectAIController::APhotoSubjectAIController()
{
	PrimaryActorTick.bCanEverTick = true;

    PerceptionComponent = CreateDefaultSubobject<UPCPerceptionComponent>(TEXT("PerceptionComponent"));
    SelfSpecies = ESubjectName::NO_NAME;
}

void APhotoSubjectAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    UpdateMoods(DeltaSeconds);
    TryUpdateBehavior();

#ifdef PC_DEBUG_LOGS
    FString NewLogString = FString::Printf(
        TEXT("PhotoSubjectAIController:: AlertLevel: %s, CurrentBehavior: %s, Alertness: %.00f/%.00f, Alarm: %.00f/%.00f, Aggression: %.00f/%.00f, AlertStimuli: %i, AlarmStimuli: %i, AggroStimuli: %i"),
        *(PCAlertLevel::EnumToString(AlertLevel)),
        *(PCPhotoSubjectBehavior::EnumToString(ActiveBehavior)),
        Alertness, AlertnessThreshold,
        Alarm, AlarmThreshold,
        Aggression, AggressionThreshold,
        ActiveAlertStimuli, ActiveAlarmStimuli, ActiveAggressionStimuli);

    if (NewLogString != LogString)
    {
        LogString = NewLogString;
        UE_LOG(LogTemp, Display, TEXT("%s"), *LogString);
    }
#endif
}

void APhotoSubjectAIController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTree != nullptr) {RunBehaviorTree(BehaviorTree);}
    if (PerceptionComponent)
    {
        PerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &APhotoSubjectAIController::OnPerceptionInfoUpdated);
    }

    SetAlertLevelKeyValue(BBKEY_ALERT_LEVEL, EAlertLevel::CALM);
    SetBehaviorKeyValue(BBKEY_ACTIVE_BEHAVIOR, EPhotoSubjectBehavior::IDLE);
}

void APhotoSubjectAIController::HandleHearingStimulus(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    if (GetTargetSpecies(UpdateInfo) == SelfSpecies){return;}
    if (UpdateInfo.Stimulus.IsExpired()) {ActiveAlertStimuli = false;}
    else if (UpdateInfo.Stimulus.IsActive())
    {
        float DistanceFromStimulus = FVector::Distance(UpdateInfo.Stimulus.ReceiverLocation, UpdateInfo.Stimulus.StimulusLocation);
        float AttenuatedStrength = FMath::Max(UpdateInfo.Stimulus.Strength - 20 * log10f(DistanceFromStimulus/100), 0.0f);

#ifdef PC_DEBUG_LOGS
        UE_LOG(LogTemp, Display, TEXT("PhotoSubjectAIController:: HearingStimulus: Receiver: %s, Instigator: %s, Strength: %.00f, Attenuated: %.00f, Noticed: %s"),
            *(GetCharacter() == nullptr ? FString("Error") : GetCharacter()->GetActorNameOrLabel()),
            *((UpdateInfo.Target == nullptr) ? FString("Error") : UpdateInfo.Target->GetActorNameOrLabel()),
            UpdateInfo.Stimulus.Strength,
            AttenuatedStrength,
            *((AttenuatedStrength < NoticeSoundThreshold) ? FString("false") : FString("true")));
#endif

        if (AttenuatedStrength < NoticeSoundThreshold) {return;}
        ActiveAlertStimuli = true;

        AActor* SourceActor = UpdateInfo.Target.Get();
        CHECK_NULLPTR_RET(SourceActor, LogAIPerception, "PhotoSubjectAIController:: Stimulus Target is null!");
        LastHeardActor = SourceActor;
        SetAlertness(Alertness + AlertnessMultiplier * (AttenuatedStrength - NoticeSoundThreshold));
    }
}

void APhotoSubjectAIController::HandleSightStimulus(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    UE_LOG(LogTemp, Display, TEXT("PhotoSubjectAIController:: HandleSightStimulus"));
    bool bIsActiveStimulus = UpdateInfo.Stimulus.IsActive() && !UpdateInfo.Stimulus.IsExpired();

    if (UpdateInfo.Target != nullptr)
    {
        ESubjectName TargetSpecies = GetTargetSpecies(UpdateInfo);
        if (TargetSpecies == SelfSpecies) {return;}

        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UpdateInfo.Target);
        if (Predators.Contains(TargetSpecies) || (PlayerCharacter != nullptr && ReactionToPlayer == EAlertLevel::ALARMED))
        {
            ActiveAlarmStimuli = bIsActiveStimulus;
            LastSeenPredator = UpdateInfo.Target.Get();
        }
        else if (Prey.Contains(TargetSpecies) || (PlayerCharacter != nullptr && ReactionToPlayer == EAlertLevel::AGGRO))
        {
            ActiveAggressionStimuli = bIsActiveStimulus;
            LastSeenPrey = UpdateInfo.Target.Get();
        }
    }
}

void APhotoSubjectAIController::UpdateMoods(float DeltaSeconds)
{
    if (!ActiveAlertStimuli)
    {
        SetAlertness(Alertness - AlertnessDecay * DeltaSeconds);
        if (!ActiveAlarmStimuli) {SetAlarm(Alarm - AlarmDecay * DeltaSeconds);}
        if (!ActiveAggressionStimuli) {SetAggression(Aggression - AggressionDecay * DeltaSeconds);}
    }

    if (ActiveAlarmStimuli)
    {
        float DistanceFromTarget = GetDistanceFromTarget(LastSeenPredator);
        float DistanceFactor = FMath::Clamp((1.25 - DistanceFromTarget / 4000), 0.5, 1.0);
        SetAlarm(Alarm + AlarmIncrement * DeltaSeconds * DistanceFactor);
    }
    
    if (ActiveAggressionStimuli)
    {
        float DistanceFromTarget = GetDistanceFromTarget(LastSeenPrey);
        float DistanceFactor = FMath::Clamp((1.25 - DistanceFromTarget / 4000), 0.5, 1.0);
        SetAggression(Aggression + AggressionIncrement * DeltaSeconds * DistanceFactor);
    }
}

void APhotoSubjectAIController::UpdateBehavior()
{
#ifdef PC_DEBUG_LOGS
    UE_LOG(LogPhotoSubjectAI, Warning, TEXT("PhotoSubjectAIController:: UpdateBehavior()"));
#endif

    LastBehaviorUpdateTime = GetWorld()->GetTimeSeconds();

    EAlertLevel NewAlertLevel{EAlertLevel::CALM};

    if (Alarm >= AlarmThreshold)
    {
        NewAlertLevel = EAlertLevel::ALARMED;
        SetObjectKeyValue(BBKEY_REACTION_TARGET, LastSeenPredator);
    }
    else if (Aggression >= AggressionThreshold)
    {
        NewAlertLevel = EAlertLevel::AGGRO;
        SetObjectKeyValue(BBKEY_REACTION_TARGET, LastSeenPrey);
    }
    else if (Alertness >= AlertnessThreshold)
    {
        NewAlertLevel = EAlertLevel::ALERT;
        SetObjectKeyValue(BBKEY_REACTION_TARGET, LastHeardActor);
    }
    else
    {
        GetBlackboardComponent()->ClearValue(BBKEY_REACTION_TARGET);
    }

    SetAlertLevelKeyValue(BBKEY_ALERT_LEVEL, NewAlertLevel);
}

void APhotoSubjectAIController::TryUpdateBehavior()
{
    float CurrentTimeSeconds = GetWorld()->GetTimeSeconds();
    if (!CheckCurrentBehavior() || CurrentTimeSeconds - LastBehaviorUpdateTime >= BehaviorUpdateFrequency)
    {
        UpdateBehavior();
    }
}

bool APhotoSubjectAIController::CheckCurrentBehavior()
{
    bool bResult{true};
    AActor* ReactionTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(BBKEY_REACTION_TARGET));
    if (ReactionTarget != nullptr && !ReactionTarget->IsValidLowLevel())
    {
        GetBlackboardComponent()->ClearValue(BBKEY_REACTION_TARGET);
        if (AlertLevel == EAlertLevel::AGGRO || AlertLevel == EAlertLevel::ALARMED) {bResult = false;}
    }
    return bResult;
}

ESubjectName APhotoSubjectAIController::GetTargetSpecies(const FActorPerceptionUpdateInfo &UpdateInfo)
{
    if (UpdateInfo.Target != nullptr)
    {
        if(UPhotoSubjectDataComponent* DataComponent = UpdateInfo.Target->FindComponentByClass<UPhotoSubjectDataComponent>())
        {
            return DataComponent->GetSubjectName();
        }
    }
    return ESubjectName::NO_NAME;
}

float APhotoSubjectAIController::GetDistanceFromTarget(AActor* TargetActor)
{
    ACharacter* ControlledCharacter = GetCharacter();
    if (TargetActor == nullptr || ControlledCharacter == nullptr) {return 0.0f;}

    return FVector::Distance(TargetActor->GetActorLocation(), ControlledCharacter->GetActorLocation());
}

void APhotoSubjectAIController::SetAlertLevelKeyValue(const FName &KeyName, EAlertLevel InAlertLevel)
{
    if (KeyName == BBKEY_ALERT_LEVEL)
    {
        if (InAlertLevel == AlertLevel)
        {
            LastBehavior = ActiveBehavior;
            // SetBehaviorKeyValue(BBKEY_ACTIVE_BEHAVIOR, EPhotoSubjectBehavior::RESTART_LAST);
        }
        else {AlertLevel = InAlertLevel;}
    }
    GetBlackboardComponent()->SetValueAsEnum(KeyName, InAlertLevel);
}

void APhotoSubjectAIController::SetBehaviorKeyValue(const FName& KeyName, EPhotoSubjectBehavior InBehavior)
{
#ifdef PC_DEBUG_LOGS
    UE_LOG(LogTemp, Error, TEXT("PhotoSubjectAIController:: SetBehaviorKeyValue: InBehavior: %s"), *(PCPhotoSubjectBehavior::EnumToString(InBehavior)));
#endif

    EPhotoSubjectBehavior NewBehavior = InBehavior;
    if (KeyName == BBKEY_ACTIVE_BEHAVIOR)
    {
        // if (ActiveBehavior == EPhotoSubjectBehavior::RESTART_LAST)
        // {
        //     NewBehavior = LastBehavior;
        //     LastBehavior = EPhotoSubjectBehavior::NONE;
        // }
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
    // if (AlertLevel > InAlertLevel) {return;}
    
    // UPhotoSubjectDataComponent* DataComponent = ReactionSource->FindComponentByClass<UPhotoSubjectDataComponent>();
    // EAlertLevel NewAlertLevel = InAlertLevel;

    // if (DataComponent != nullptr)
    // {
    //     for (ESubjectName PreyName : Prey)
    //     {
    //         if (DataComponent->GetSubjectName() == PreyName && NewAlertLevel == EAlertLevel::ALARMED)
    //         {
    //             NewAlertLevel = EAlertLevel::AGGRO;
    //             break;
    //         }
    //     }
    // }

    // else if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ReactionSource))
    // {
    //     if (bIsHostileToPlayer && NewAlertLevel == EAlertLevel::ALARMED)
    //     {
    //         NewAlertLevel = EAlertLevel::AGGRO;
    //     }
    // }

    // SetAlertLevelKeyValue(BBKEY_ALERT_LEVEL, NewAlertLevel);
    // SetObjectKeyValue(BBKEY_REACTION_TARGET, ReactionSource);
}

void APhotoSubjectAIController::OnPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(GetWorld());
    CHECK_NULLPTR_RET(PerceptionSystem, LogAIPerception, "PhotoSubjectAIController:: AIPerceptionSystem not found!");

    TSubclassOf<UAISense> StimulusSenseClass = PerceptionSystem->GetSenseClassForStimulus(this, UpdateInfo.Stimulus);
    if (StimulusSenseClass == UAISense_Hearing::StaticClass()) {HandleHearingStimulus(UpdateInfo);}
    else if (StimulusSenseClass == UAISense_Sight::StaticClass()) {HandleSightStimulus(UpdateInfo);}
}
