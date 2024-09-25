// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AlertLevel.h"
#include "PhotoSubjectBehavior.h"
#include "PhotoSubjectAIController.generated.h"

#define BBKEY_HOME_LOCATION     TEXT("HomeLocation")
#define BBKEY_TARGET_LOCATION   TEXT("TargetLocation")
#define BBKEY_ALERT_LEVEL       TEXT("AlertLevel")
#define BBKEY_ACTIVE_BEHAVIOR   TEXT("ActiveBehavior")
#define BBKEY_REACTION_TARGET 	TEXT("ReactionTarget")

enum ESubjectName;
struct FActorPerceptionUpdateInfo;


UCLASS()
class PROJECTCALM_API APhotoSubjectAIController : public AAIController
{
	GENERATED_BODY()

public:
	APhotoSubjectAIController();
	APhotoSubjectAIController(const FObjectInitializer &ObjectInitializer) : APhotoSubjectAIController(){};
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = Behavior)
	UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, Category = Reactions)
	TArray<TEnumAsByte<ESubjectName>> Predators;
	UPROPERTY(EditAnywhere, Category = Reactions)
	TArray<TEnumAsByte<ESubjectName>> Prey;
	UPROPERTY(EditAnywhere, Category = Reactions)
	TEnumAsByte<EAlertLevel> ReactionToPlayer{EAlertLevel::CALM};
	UPROPERTY(EditAnywhere, Category = Reactions)
	float NoticeSoundThreshold{12.0f};

	UPROPERTY(EditAnywhere, Category = Behavior)
	float BehaviorUpdateFrequency{0.5f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AlertnessThreshold{100.0f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AlertnessMultiplier{3.0f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AlertnessDecay{3.333f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AlarmThreshold{100.0f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AlarmIncrement{65.0f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AlarmDecay{5.0f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AggressionThreshold{100.0f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AggressionIncrement{65.0f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AggressionDecay{6.667f};

	ESubjectName SelfSpecies;
	float LastBehaviorUpdateTime{0.0f};
	AActor* LastHeardActor{nullptr};
	AActor* LastSeenPrey{nullptr};
	AActor* LastSeenPredator{nullptr};

	float Alertness{0.0f};
	float Alarm{0.0f};
	float Aggression{0.0f};

	bool ActiveAlertStimuli{false};
	bool ActiveAlarmStimuli{false};
	bool ActiveAggressionStimuli{false};
	
	EAlertLevel AlertLevel{EAlertLevel::CALM};
	EPhotoSubjectBehavior ActiveBehavior{EPhotoSubjectBehavior::NONE};
	EPhotoSubjectBehavior LastBehavior{EPhotoSubjectBehavior::NONE};
	FVector HomeLocation{FVector::ZeroVector};
	FVector TargetLocation{FVector::ZeroVector};

	void SetAlertness(float InAlertness) {Alertness = FMath::Max(0, InAlertness);}
	void SetAlarm(float InAlarm) {Alarm = FMath::Max(0, InAlarm);}
	void SetAggression(float InAggression) {Aggression = FMath::Max(0, InAggression);}

	void HandleHearingStimulus(const FActorPerceptionUpdateInfo& UpdateInfo);
	void HandleSightStimulus(const FActorPerceptionUpdateInfo& UpdateInfo);

	void UpdateMoods(float DeltaSeconds);
	void UpdateBehavior();
	void TryUpdateBehavior();
	bool CheckCurrentBehavior();
	ESubjectName GetTargetSpecies(const FActorPerceptionUpdateInfo& UpdateInfo);
	float GetDistanceFromTarget(AActor* TargetActor);

public:
	UFUNCTION(BlueprintCallable)
	EAlertLevel GetAlertLevel(){return AlertLevel;}
	UFUNCTION(BlueprintCallable)
	EPhotoSubjectBehavior GetActiveBehavior(){return ActiveBehavior;}
	UFUNCTION(BlueprintCallable)
	FVector GetHomeLocation(){return HomeLocation;}

	void SetSelfSpecies(ESubjectName InSpecies) {SelfSpecies = InSpecies;}

	void SetAlertLevelKeyValue(const FName& KeyName, EAlertLevel InAlertLevel);
	void SetBehaviorKeyValue(const FName& KeyName, EPhotoSubjectBehavior InBehavior);
	void SetVectorKeyValue(const FName& KeyName, FVector InVector);
	void SetBoolKeyValue(const FName& KeyName, bool bInValue);
	void SetObjectKeyValue(const FName& KeyName, UObject* InObject);
	
	void DetermineReaction(EAlertLevel InAlertLevel, AActor* ReactionSource);

	UFUNCTION()
	void OnPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

	
#if WITH_EDITORONLY_DATA
private:
    FString LogString;
#endif

};
