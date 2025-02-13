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
#define BBKEY_IS_DEAD		 	TEXT("bIsDead")

enum ESubjectName : uint8;
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
	float AggressionIncrement{60.0f};
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AggressionDecay{30.0f};

	/*If negative, cannot attack.*/
	UPROPERTY(EditAnywhere, Category = Behavior)
	float AttackCooldown{-1.0f};
	float LastAttackTime{0.0f};

	ESubjectName SelfSpecies;
	float LastBehaviorUpdateTime{0.0f};
	AActor* LastHeardActor{nullptr};
	AActor* LastSeenPrey{nullptr};
	AActor* LastSeenPredator{nullptr};

	float Alertness{0.0f};
	float Alarm{0.0f};
	float Aggression{0.0f};

	bool ActiveAlertStimulus{false};
	bool ActiveAlarmStimulus{false};
	bool ActiveAggressionStimulus{false};
	
	EAlertLevel AlertLevel{EAlertLevel::CALM};
	EPhotoSubjectBehavior ActiveBehavior{EPhotoSubjectBehavior::NONE};
	EPhotoSubjectBehavior LastBehavior{EPhotoSubjectBehavior::NONE};
	FVector HomeLocation{FVector::ZeroVector};
	FVector TargetLocation{FVector::ZeroVector};

	void SetAlertness(float InAlertness);
	void SetAlarm(float InAlarm);
	void SetAggression(float InAggression);

	void HandleHearingStimulus(const FActorPerceptionUpdateInfo& UpdateInfo);
	void HandleSightStimulus(const FActorPerceptionUpdateInfo& UpdateInfo);

	void UpdateMoods(float DeltaSeconds);
	void UpdateBehavior();
	void TryUpdateBehavior();
	bool CheckCurrentBehavior();
	ESubjectName GetTargetSpecies(const FActorPerceptionUpdateInfo& UpdateInfo);
	float GetDistanceFromTarget(AActor* TargetActor);
	bool IsTargetDead(AActor* TargetActor) const;

protected:
	UFUNCTION()
	virtual void HandleDeath(FString DamageMessage);

public:
	UFUNCTION(BlueprintPure)
	EAlertLevel GetAlertLevel() const {return AlertLevel;}
	UFUNCTION(BlueprintPure)
	EPhotoSubjectBehavior GetActiveBehavior() const {return ActiveBehavior;}
	UFUNCTION(BlueprintPure)
	FVector GetHomeLocation() const {return HomeLocation;}
	UFUNCTION(BlueprintPure)
	AActor* GetCurrentTarget() const;
	UFUNCTION(BlueprintPure)
	bool CanAttack() const;

	void ActivateAttackCooldown();

	void SetSelfSpecies(ESubjectName InSpecies) {SelfSpecies = InSpecies;}

	void SetAlertLevelKeyValue(const FName& KeyName, EAlertLevel InAlertLevel);
	void SetBehaviorKeyValue(const FName& KeyName, EPhotoSubjectBehavior InBehavior);
	void SetVectorKeyValue(const FName& KeyName, FVector InVector);
	void SetBoolKeyValue(const FName& KeyName, bool bInValue);
	void SetObjectKeyValue(const FName& KeyName, UObject* InObject);

	UFUNCTION()
	void OnPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

	
#if WITH_EDITORONLY_DATA
private:
    FString LogString;
#endif

};
