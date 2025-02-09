// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LookAt.h"
#include "ProjectCalm/Utilities/PCMath.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


#define MIN_RESPONSE_ANGLE 10

void UBTTask_LookAt::InitializeMemory(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, EBTMemoryInit::Type InitType) const
{
    Super::InitializeMemory(OwnerComp, NodeMemory, InitType);

    FBTLookAtTaskMemory* LookAtMemory = (FBTLookAtTaskMemory*)NodeMemory;
    LookAtMemory->RotationSpeed = -1.0f;
}

UBTTask_LookAt::UBTTask_LookAt()
{
    NodeName = TEXT("Look At");
}

EBTNodeResult::Type UBTTask_LookAt::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    bNotifyTaskFinished = true;

    AAIController* AIController = OwnerComp.GetAIOwner();
    CHECK_NULLPTR_RETVAL(AIController, LogBehaviorTree, "BTTask::LookAt:: AI CONTROLLER NOT FOUND!", EBTNodeResult::Failed);

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    CHECK_NULLPTR_RETVAL(BlackboardComponent, LogBehaviorTree, "BTTask::LookAt:: BLACKBOARD COMPONENT NOT FOUND!", EBTNodeResult::Failed);

    FVector FocusLocation{FVector::ZeroVector};
    if (BlackboardKey.SelectedKeyType.Get() == UBlackboardKeyType_Object::StaticClass())
    {
        AActor* ReferenceActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(BlackboardKey.SelectedKeyName));
        CHECK_NULLPTR_RETVAL(ReferenceActor, LogBehaviorTree, "BTTask::LookAt:: REFERENCE OBJECT NOT AN ACTOR!", EBTNodeResult::Failed);

        FocusLocation = ReferenceActor->GetActorLocation();
    }
    else if (BlackboardKey.SelectedKeyType.Get() == UBlackboardKeyType_Object::StaticClass())
    {
        FocusLocation = BlackboardComponent->GetValueAsVector(BlackboardKey.SelectedKeyName);
    }

    float AngleToTarget = FMath::Abs(PCMath::CalculateLocationAngle2D(AIController->GetPawn(), FocusLocation));
    if (AngleToTarget < MIN_RESPONSE_ANGLE) {return EBTNodeResult::Succeeded;}

    SetPawnRotationSettings(OwnerComp, (FBTLookAtTaskMemory*)NodeMemory, false, AngleToTarget);
    return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UBTTask_LookAt::AbortTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    SetPawnRotationSettings(OwnerComp, (FBTLookAtTaskMemory*)NodeMemory, true);
    return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_LookAt::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    SetPawnRotationSettings(OwnerComp, (FBTLookAtTaskMemory*)NodeMemory, true);
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_LookAt::SetPawnRotationSettings(UBehaviorTreeComponent& OwnerComp, FBTLookAtTaskMemory* NodeMemory, bool bReset, float AngleToTarget)
{
    if (NodeMemory == nullptr) {return;}

    AAIController* AIController = OwnerComp.GetAIOwner();
    CHECK_NULLPTR_RET(AIController, LogBehaviorTree, "BTTask::LookAt:: AI CONTROLLER NOT FOUND!");
    
    APawn* AIPawn = AIController->GetPawn();
    CHECK_NULLPTR_RET(AIPawn, LogBehaviorTree, "BTTask::LookAt:: PAWN NOT FOUND!");

    ACharacter* AICharacter = Cast<ACharacter>(AIPawn);
    CHECK_NULLPTR_RET(AICharacter, LogBehaviorTree, "BTTask::LookAt:: CHARACTER NOT FOUND!");
    
    UCharacterMovementComponent* AIMovement = Cast<UCharacterMovementComponent>(AICharacter->GetMovementComponent());
    CHECK_NULLPTR_RET(AIMovement, LogBehaviorTree, "BTTask::LookAt:: MOVEMENT COMPONENT NOT FOUND!");

    if (NodeMemory->RotationSpeed < 0) {NodeMemory->RotationSpeed = AIMovement->RotationRate.Yaw;}

    if (!bReset)
    {
        float NewRotationSpeed = FMath::Lerp(
            NodeMemory->RotationSpeed / 12,
            NodeMemory->RotationSpeed,
            FMath::Clamp(AngleToTarget / 120, 0, 1));

        AIMovement->RotationRate.Yaw = NewRotationSpeed;
    }
    else {AIMovement->RotationRate.Yaw = NodeMemory->RotationSpeed;}

    AIMovement->bOrientRotationToMovement = bReset;
    AIMovement->bUseControllerDesiredRotation = !bReset;
}
