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

enum ESubjectName;


UCLASS()
class PROJECTCALM_API APhotoSubjectAIController : public AAIController
{
	GENERATED_BODY()

public:
	APhotoSubjectAIController();
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

	EAlertLevel AlertLevel{EAlertLevel::CALM};
	EPhotoSubjectBehavior ActiveBehavior{EPhotoSubjectBehavior::NONE};
	EPhotoSubjectBehavior LastBehavior{EPhotoSubjectBehavior::NONE};
	FVector HomeLocation{FVector::ZeroVector};
	FVector TargetLocation{FVector::ZeroVector};

public:
	UFUNCTION(BlueprintCallable)
	EAlertLevel GetAlertLevel(){return AlertLevel;}
	UFUNCTION(BlueprintCallable)
	EPhotoSubjectBehavior GetActiveBehavior(){return ActiveBehavior;}
	UFUNCTION(BlueprintCallable)
	FVector GetHomeLocation(){return HomeLocation;}

	void SetAlertLevelKeyValue(const FName& KeyName, EAlertLevel InAlertLevel);
	void SetBehaviorKeyValue(const FName& KeyName, EPhotoSubjectBehavior InBehavior);
	void SetVectorKeyValue(const FName& KeyName, FVector InVector);
	void SetBoolKeyValue(const FName& KeyName, bool bInValue);
	
	void DetermineReaction(EAlertLevel InAlertLevel, AActor* ReactionSource);

};
