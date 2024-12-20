// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif


UPlayerCharacterMovementComponent::UPlayerCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	NavAgentProps.bCanCrouch = true;
	
}

void UPlayerCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
}

// Called when the game starts
void UPlayerCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
    	
}

void UPlayerCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogPlayerCharacter, Display,
		TEXT("PlayerCharacterMovementComponent:: At OnMovementUpdated bWantsToCrouch = %s, bWantsToSprint = %s, bWantsToRestrict = %s"),
		(bWantsToCrouch ? *FString("true") : *FString("false")),
		(bWantsToSprint ? *FString("true") : *FString("false")),
		(bWantsToRestrict ? *FString("true") : *FString("false")));
	UE_LOG(LogPlayerCharacter, Display, TEXT("    MovementMode = %u, CustomMovement = %u"),
		(uint8)MovementMode.GetValue(),
		(uint8)CustomMovementMode);
#endif

	if (bWantsToRestrict && IsMovingOnGround())
	{
		if (IsCrouching()) {SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovement::CROUCH_RESTRICTED);}
		else {SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovement::WALK_RESTRICTED);}
	}
	else if (bWantsToSprint && IsMovingOnGround())
	{
		if (bWantsToCrouch || IsCrouching()) {SetMovementMode(EMovementMode::MOVE_Walking);}
		else {SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovement::SPRINT);}		
	}

	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UPlayerCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case ECustomMovement::CROUCH_RESTRICTED :
		PhysCrouchRestricted(deltaTime, Iterations);
		break;
	case ECustomMovement::WALK_RESTRICTED :
		PhysWalkRestricted(deltaTime, Iterations);
		break;
	case ECustomMovement::SPRINT :
		PhysSprint(deltaTime, Iterations);
		break;
	
	default:
		break;
	}

	Super::PhysCustom(deltaTime, Iterations);
}

void UPlayerCharacterMovementComponent::PhysSprint(float deltaTime, int32 Iterations)
{
	if (!IsSprinting())
	{
		SetWantsToSprint(false);
		SetMovementMode(EMovementMode::MOVE_Walking);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	PhysWalking(deltaTime, Iterations);
}

void UPlayerCharacterMovementComponent::PhysWalkRestricted(float deltaTime, int32 Iterations)
{
	if (!IsRestrictedWalking())
	{
		SetWantsToRestrict(false);
		SetMovementMode(EMovementMode::MOVE_Walking);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	PhysWalking(deltaTime, Iterations);
}

void UPlayerCharacterMovementComponent::PhysCrouchRestricted(float deltaTime, int32 Iterations)
{
	if (!IsRestrictedCrouching())
	{
		if (bWantsToRestrict)
		{
			UnCrouch(false);
			SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovement::WALK_RESTRICTED);
			StartNewPhysics(deltaTime, Iterations);
			return;
		}

		if (bWantsToCrouch)
		{
			SetWantsToRestrict(false);
			SetMovementMode(EMovementMode::MOVE_Walking);
			StartNewPhysics(deltaTime, Iterations);
			return;
		}
	}

	PhysWalking(deltaTime, Iterations);
}

float UPlayerCharacterMovementComponent::GetMaxSpeed() const
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogPlayerCharacter, Display,
		TEXT("PlayerCharacterMovementComponent:: At GetMaxSpeed MovementMode = %u, CustomMovement = %u"),
		(uint8)MovementMode.GetValue(),
		(uint8)CustomMovementMode);
#endif

    if (MovementMode == EMovementMode::MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case ECustomMovement::SPRINT:
			return MaxSprintSpeed;
		case ECustomMovement::WALK_RESTRICTED:
			return MaxWalkRestrictedSpeed;
		case ECustomMovement::CROUCH_RESTRICTED:
			return MaxCrouchRestrictedSpeed;
		
		default:
			break;
		}
	}

	return Super::GetMaxSpeed();
}

bool UPlayerCharacterMovementComponent::CanAttemptJump() const
{
    return Super::CanAttemptJump() && !bWantsToRestrict;
}

void UPlayerCharacterMovementComponent::SetWantsToSprint(bool bValue)
{
	bWantsToSprint = bValue && bIsSprintAllowed;
}

void UPlayerCharacterMovementComponent::SetWantsToRestrict(bool bValue)
{
	bWantsToRestrict = bValue;
	bIsSprintAllowed = !bValue;
}

bool UPlayerCharacterMovementComponent::IsMovingOnGround() const
{
    return Super::IsMovingOnGround() \
		|| ((CustomMovementMode == ECustomMovement::CROUCH_RESTRICTED \
		|| CustomMovementMode == ECustomMovement::WALK_RESTRICTED \
		|| CustomMovementMode == ECustomMovement::SPRINT) \
		&& UpdatedComponent);
}

bool UPlayerCharacterMovementComponent::IsSprinting() const
{
    return bWantsToSprint && !bWantsToRestrict && IsMovingOnGround() \
	&& MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == ECustomMovement::SPRINT;
}

bool UPlayerCharacterMovementComponent::IsRestrictedWalking() const
{
    return bWantsToRestrict && !bWantsToCrouch && IsMovingOnGround() \
	&& MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == ECustomMovement::WALK_RESTRICTED;
}

bool UPlayerCharacterMovementComponent::IsRestrictedCrouching() const
{
    return bWantsToRestrict && bWantsToCrouch && IsCrouching() \
	&& MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == ECustomMovement::CROUCH_RESTRICTED;
}
