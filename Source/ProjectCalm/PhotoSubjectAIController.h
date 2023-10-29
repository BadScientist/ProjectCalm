// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AlertLevel.h"
#include "PhotoSubjectBehavior.h"
#include "PhotoSubjectAIController.generated.h"


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
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree;

	EAlertLevel AlertLevel;
	EPhotoSubjectBehavior ActiveBehavior;
	FVector HomeLocation{FVector::ZeroVector};

public:
	UFUNCTION(BlueprintCallable)
	EAlertLevel GetAlertLevel(){return AlertLevel;}
	UFUNCTION(BlueprintCallable)
	EPhotoSubjectBehavior GetActiveBehavior(){return ActiveBehavior;}
	UFUNCTION(BlueprintCallable)
	FVector GetHomeLocation(){return HomeLocation;}

	void SetAlertLevel(EAlertLevel InAlertLevel);
	void SetActiveBehavior(EPhotoSubjectBehavior InBehavior);
	void SetHomeLocation(FVector InLocation);

};
