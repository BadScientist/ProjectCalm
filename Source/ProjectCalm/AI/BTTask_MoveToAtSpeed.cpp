// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToAtSpeed.h"
#include "AIController.h"
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
    if (AIController == nullptr) {return;}

    ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
    if (AICharacter == nullptr) {return;}

    UCharacterMovementComponent* AIMovement = Cast<UCharacterMovementComponent>(AICharacter->GetMovementComponent());
    if (AIMovement == nullptr) {return;}

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
