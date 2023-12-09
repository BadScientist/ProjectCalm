// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"
#include "BTTask_LookAt.generated.h"

struct FBTLookAtTaskMemory : public FBTFocusMemory
{
	float RotationSpeed = -1.0f;
};

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTTask_LookAt : public UBTTask_RotateToFaceBBEntry
{
	GENERATED_BODY()

	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const;

public:
	UBTTask_LookAt();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	// virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTLookAtTaskMemory); }

protected:
	void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);

private:
	void SetPawnRotationSettings(UBehaviorTreeComponent& OwnerComp, FBTLookAtTaskMemory* NodeMemory, bool bReset = false, float AngleToTarget = 0.0f);
	
};
