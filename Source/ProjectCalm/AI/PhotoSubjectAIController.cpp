// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoSubjectAIController.h"
#include "PCPerceptionComponent.h"
#include "ProjectCalm/Photos/PhotoSubjectDataComponent.h"
#include "ProjectCalm/Characters/HealthComponent.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Photos/PhotoSubjectName.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif


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

#ifdef LOCAL_DEBUG_LOGS
    FString NewLogString = FString::Printf(
        TEXT("PhotoSubjectAIController:: AlertLevel: %s, CurrentBehavior: %s, Alertness: %.00f/%.00f, Alarm: %.00f/%.00f, Aggression: %.00f/%.00f, AlertStimuli: %i, AlarmStimuli: %i, AggroStimuli: %i"),
        *(PCAlertLevel::EnumToString(AlertLevel)),
        *(PCPhotoSubjectBehavior::EnumToString(ActiveBehavior)),
        Alertness, AlertnessThreshold,
        Alarm, AlarmThreshold,
        Aggression, AggressionThreshold,
        ActiveAlertStimulus, ActiveAlarmStimulus, ActiveAggressionStimulus);

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

    if (ACharacter* ControlledCharacter = GetCharacter())
    {
        if (UHealthComponent* HealthComponent = ControlledCharacter->FindComponentByClass<UHealthComponent>())
        {
            HealthComponent->OnDeath.AddDynamic(this, &APhotoSubjectAIController::HandleDeath);
        }
    }

    SetAlertLevelKeyValue(BBKEY_ALERT_LEVEL, EAlertLevel::CALM);
    SetBehaviorKeyValue(BBKEY_ACTIVE_BEHAVIOR, EPhotoSubjectBehavior::IDLE);
}

void APhotoSubjectAIController::HandleHearingStimulus(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    if (GetTargetSpecies(UpdateInfo) == SelfSpecies){return;}
    if (UpdateInfo.Stimulus.IsExpired()) {ActiveAlertStimulus = false;}
    else if (UpdateInfo.Stimulus.IsActive())
    {
        float DistanceFromStimulus = FVector::Distance(UpdateInfo.Stimulus.ReceiverLocation, UpdateInfo.Stimulus.StimulusLocation);
        float AttenuatedStrength = FMath::Max(UpdateInfo.Stimulus.Strength - 20 * log10f(DistanceFromStimulus/100), 0.0f);

#ifdef LOCAL_DEBUG_LOGS
        UE_LOG(LogTemp, Display, TEXT("PhotoSubjectAIController:: HearingStimulus: Receiver: %s, Instigator: %s, Strength: %.00f, Attenuated: %.00f, Noticed: %s"),
            *(GetCharacter() == nullptr ? FString("Error") : GetCharacter()->GetActorNameOrLabel()),
            *((UpdateInfo.Target == nullptr) ? FString("Error") : UpdateInfo.Target->GetActorNameOrLabel()),
            UpdateInfo.Stimulus.Strength,
            AttenuatedStrength,
            *((AttenuatedStrength < NoticeSoundThreshold) ? FString("false") : FString("true")));
#endif

        if (AttenuatedStrength < NoticeSoundThreshold) {return;}
        ActiveAlertStimulus = true;

        AActor* SourceActor = UpdateInfo.Target.Get();
        CHECK_NULLPTR_RET(SourceActor, LogAIPerception, "PhotoSubjectAIController:: Stimulus Target is null!");
        LastHeardActor = SourceActor;
        SetAlertness(Alertness + AlertnessMultiplier * (AttenuatedStrength - NoticeSoundThreshold));
    }
}

void APhotoSubjectAIController::HandleSightStimulus(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    bool bIsActiveStimulus = UpdateInfo.Stimulus.IsActive() && !UpdateInfo.Stimulus.IsExpired();

    if (UpdateInfo.Target.Get() != nullptr)
    {
        ESubjectName TargetSpecies = GetTargetSpecies(UpdateInfo);
        if (TargetSpecies == SelfSpecies) {return;}

        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UpdateInfo.Target);
        if (Predators.Contains(TargetSpecies) || (PlayerCharacter != nullptr && ReactionToPlayer == EAlertLevel::ALARMED))
        {
            ActiveAlarmStimulus = bIsActiveStimulus && !IsTargetDead(UpdateInfo.Target.Get());
            if (ActiveAlarmStimulus) {LastSeenPredator = UpdateInfo.Target.Get();}
        }
        else if (Prey.Contains(TargetSpecies) || (PlayerCharacter != nullptr && ReactionToPlayer == EAlertLevel::AGGRO))
        {
            ActiveAggressionStimulus = bIsActiveStimulus && !IsTargetDead(UpdateInfo.Target.Get());
            if (ActiveAggressionStimulus) {LastSeenPrey = UpdateInfo.Target.Get();}
        }
    }
}

void APhotoSubjectAIController::UpdateMoods(float DeltaSeconds)
{
    if (LastSeenPredator != nullptr) {ActiveAlarmStimulus = ActiveAlarmStimulus && !IsTargetDead(LastSeenPredator);}
    if (LastSeenPrey != nullptr) {ActiveAggressionStimulus = ActiveAggressionStimulus && !IsTargetDead(LastSeenPrey);}

    if (!ActiveAlertStimulus)
    {
        SetAlertness(Alertness - AlertnessDecay * DeltaSeconds);
        if (!ActiveAlarmStimulus) {SetAlarm(Alarm - AlarmDecay * DeltaSeconds);}
        if (!ActiveAggressionStimulus) {SetAggression(Aggression - AggressionDecay * DeltaSeconds);}
    }

    if (ActiveAlarmStimulus)
    {
        float DistanceFromTarget = GetDistanceFromTarget(LastSeenPredator);
        float DistanceFactor = FMath::Clamp((1.25 - DistanceFromTarget / 4000), 0.5, 1.0);
        SetAlarm(Alarm + AlarmIncrement * DeltaSeconds * DistanceFactor);
    }
    
    if (ActiveAggressionStimulus)
    {
        float DistanceFromTarget = GetDistanceFromTarget(LastSeenPrey);
        float DistanceFactor = FMath::Clamp((1.25 - DistanceFromTarget / 4000), 0.5, 1.0);
        SetAggression(Aggression + AggressionIncrement * DeltaSeconds * DistanceFactor);
    }
}

void APhotoSubjectAIController::UpdateBehavior()
{
    UWorld* World = GetWorld();
    CHECK_NULLPTR_RET(World, LogPhotoSubjectAI, "PhotoSubjectAIController:: Failed to get World!");
    LastBehaviorUpdateTime = World->GetTimeSeconds();

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
    UWorld* World = GetWorld();
    CHECK_NULLPTR_RET(World, LogPhotoSubjectAI, "PhotoSubjectAIController:: Failed to get World!");

    float CurrentTimeSeconds = World->GetTimeSeconds();
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

bool APhotoSubjectAIController::IsTargetDead(AActor* TargetActor) const
{
    if (TargetActor == nullptr) {return true;}

    UHealthComponent* TargetHealth = TargetActor->FindComponentByClass<UHealthComponent>();
    if (TargetHealth == nullptr) {return true;}

    return TargetHealth->IsDead();
}

void APhotoSubjectAIController::HandleDeath(FString DamageMessage)
{
    SetBoolKeyValue(BBKEY_IS_DEAD, true);
}

AActor* APhotoSubjectAIController::GetCurrentTarget() const
{
    const UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
    CHECK_NULLPTR_RETVAL(BlackboardComponent, LogController, "PhotoSubjectAIController:: No BlackboardComp found!", nullptr);

    return Cast<AActor>(BlackboardComponent->GetValueAsObject(BBKEY_REACTION_TARGET));
}

bool APhotoSubjectAIController::CanAttack() const
{
    UWorld* World = GetWorld();
    CHECK_NULLPTR_RETVAL(World, LogPhotoSubjectAI, "PhotoSubjectAIController:: Failed to get World!", false);

    return AttackCooldown >= 0 && World->GetTimeSeconds() - LastAttackTime >= AttackCooldown && !IsTargetDead(LastSeenPrey);
}

void APhotoSubjectAIController::ActivateAttackCooldown()
{
    UWorld* World = GetWorld();
    CHECK_NULLPTR_RET(World, LogPhotoSubjectAI, "PhotoSubjectAIController:: Failed to get World!");
    LastAttackTime = World->GetTimeSeconds();
}

void APhotoSubjectAIController::SetAlertLevelKeyValue(const FName &KeyName, EAlertLevel InAlertLevel)
{
    if (KeyName == BBKEY_ALERT_LEVEL)
    {
        if (InAlertLevel == AlertLevel) {LastBehavior = ActiveBehavior;}
        else {AlertLevel = InAlertLevel;}
    }
    GetBlackboardComponent()->SetValueAsEnum(KeyName, InAlertLevel);
}

void APhotoSubjectAIController::SetBehaviorKeyValue(const FName& KeyName, EPhotoSubjectBehavior InBehavior)
{
    EPhotoSubjectBehavior NewBehavior = InBehavior;
    if (KeyName == BBKEY_ACTIVE_BEHAVIOR) {ActiveBehavior = NewBehavior;}

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

void APhotoSubjectAIController::OnPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(GetWorld());
    CHECK_NULLPTR_RET(PerceptionSystem, LogAIPerception, "PhotoSubjectAIController:: AIPerceptionSystem not found!");

    TSubclassOf<UAISense> StimulusSenseClass = PerceptionSystem->GetSenseClassForStimulus(this, UpdateInfo.Stimulus);
    if (StimulusSenseClass == UAISense_Hearing::StaticClass()) {HandleHearingStimulus(UpdateInfo);}
    else if (StimulusSenseClass == UAISense_Sight::StaticClass()) {HandleSightStimulus(UpdateInfo);}
}
