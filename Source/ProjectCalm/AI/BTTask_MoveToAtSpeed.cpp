// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToAtSpeed.h"
#include "AIController.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTask_MoveToAtSpeed::UBTTask_MoveToAtSpeed()
{
    NodeName = TEXT("Move To At Speed");
}

EBTNodeResult::Type UBTTask_MoveToAtSpeed::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    SetMovementModeAndSpeed(OwnerComp);

    return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_MoveToAtSpeed::SetMovementModeAndSpeed(UBehaviorTreeComponent &OwnerComp)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    CHECK_NULLPTR_RET(AIController, LogBehaviorTree, "UBTTask_MoveToAtSpeed:: Could not get AI Owner!");

    ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
    CHECK_NULLPTR_RET(AIController, LogBehaviorTree, "UBTTask_MoveToAtSpeed:: Could not get controlled Character!");

    UCharacterMovementComponent* AIMovement = Cast<UCharacterMovementComponent>(AICharacter->GetMovementComponent());
    CHECK_NULLPTR_RET(AIController, LogBehaviorTree, "UBTTask_MoveToAtSpeed:: Could not get Character Movement Component!");

    AIMovement->SetMovementMode(MovementMode);

    switch (MovementMode)
    {
    case EMovementMode::MOVE_Walking:
        AIMovement->MaxWalkSpeed = MovementSpeed;
        break;
    case EMovementMode::MOVE_Flying:
        AIMovement->MaxFlySpeed = MovementSpeed;
        break;
    case EMovementMode::MOVE_Swimming:
        AIMovement->MaxSwimSpeed = MovementSpeed;
        break;                
    default:
        break;
    }
}
