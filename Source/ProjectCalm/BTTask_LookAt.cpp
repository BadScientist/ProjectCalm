// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LookAt.h"
#include "PCMath.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#define MIN_RESPONSE_ANGLE 10

void UBTTask_LookAt::InitializeMemory(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, EBTMemoryInit::Type InitType) const
{
    UE_LOG(LogTemp, Warning, TEXT("Initializing NodeMemory"));
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
    if (AIController == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::LookAt:: AI CONTROLLER NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::LookAt:: BLACKBOARD COMPONENT NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    FVector FocusLocation;
    if (BlackboardKey.SelectedKeyType.Get() == UBlackboardKeyType_Object::StaticClass())
    {
        AActor* ReferenceActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(BlackboardKey.SelectedKeyName));
        if (ReferenceActor == nullptr)
        {
            UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::LookAt:: REFERENCE OBJECT NOT AN ACTOR!"));
            return EBTNodeResult::Failed;
        }

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

// void UBTTask_LookAt::OnGameplayTaskDeactivated(UGameplayTask &Task)
// {
// 	if (Task.GetTaskOwner() == this)
//     {
//         if (UBehaviorTreeComponent* OwnerComp = GetBTComponentForTask(Task))
//         {
//             SetPawnRotationSettings(*OwnerComp, LookAtTaskMemory, true);
//         }
//     }

//     Super::OnGameplayTaskDeactivated(Task);
// }

void UBTTask_LookAt::SetPawnRotationSettings(UBehaviorTreeComponent& OwnerComp, FBTLookAtTaskMemory* NodeMemory, bool bReset, float AngleToTarget)
{
    if (NodeMemory == nullptr) {return;}

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::LookAt:: AI CONTROLLER NOT FOUND!"));
        return;
    }
    
    APawn* AIPawn = AIController->GetPawn();
    if (AIPawn == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::LookAt:: PAWN NOT FOUND!"));
        return;
    }

    ACharacter* AICharacter = Cast<ACharacter>(AIPawn);
    if (AICharacter == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::LookAt:: CHARACTER NOT FOUND!"));
        return;
    }
    
    UCharacterMovementComponent* AIMovement = Cast<UCharacterMovementComponent>(AICharacter->GetMovementComponent());
    if (AIMovement == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::LookAt:: MOVEMENT COMPONENT NOT FOUND!"));
        return;
    }

    if (NodeMemory->RotationSpeed < 0) {
        NodeMemory->RotationSpeed = AIMovement->RotationRate.Yaw;
        UE_LOG(LogTemp, Warning, TEXT("STORING pawn rotation rate. Rotation Speed: %f"), AIMovement->RotationRate.Yaw);}

    if (!bReset)
    {
        float NewRotationSpeed = FMath::Lerp(
            NodeMemory->RotationSpeed / 12,
            NodeMemory->RotationSpeed,
            FMath::Clamp(AngleToTarget / 120, 0, 1));

        AIMovement->RotationRate.Yaw = NewRotationSpeed;
        UE_LOG(LogTemp, Warning, TEXT("MODIFYING pawn settings. Rotation Speed: %f"), NewRotationSpeed);
    }
    else
    {
        AIMovement->RotationRate.Yaw = NodeMemory->RotationSpeed;
        UE_LOG(LogTemp, Warning, TEXT("RESETTING pawn settings."));
    }


    AIMovement->bOrientRotationToMovement = bReset;
    AIMovement->bUseControllerDesiredRotation = !bReset;

    UE_LOG(LogTemp, Warning, TEXT("OrientToMovement:%i, UseControllerYaw:%i, UseControllerRotation: %i"),
        AIMovement->bOrientRotationToMovement,
        AIPawn->bUseControllerRotationYaw,
        AIMovement->bUseControllerDesiredRotation);
}
