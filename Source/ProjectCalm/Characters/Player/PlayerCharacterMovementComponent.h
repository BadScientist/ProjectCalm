// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacterMovementComponent.generated.h"


UENUM()
enum ECustomMovement
{
	SPRINT				= 1		UMETA(DisplayName = "Sprint"),
	WALK_RESTRICTED		= 2		UMETA(DisplayName = "Walk Restricted"),
	CROUCH_RESTRICTED	= 3		UMETA(DisplayName = "Crouch Restricted")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UPlayerCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Custom Movement", meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MaxSprintSpeed{300.0f};
	
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Custom Movement", meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MaxWalkRestrictedSpeed{150.0f};
	
	UPROPERTY(EditDefaultsOnly, Category="Character Movement: Custom Movement", meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MaxCrouchRestrictedSpeed{75.0f};

protected:
	bool bWantsToSprint{false};
	bool bWantsToRestrict{false};

	bool bIsSprintAllowed{true};

public:	
	// Sets default values for this component's properties
	UPlayerCharacterMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity);
	virtual void PhysCustom(float deltaTime, int32 Iterations);

	virtual void PhysSprint(float deltaTime, int32 Iterations);
	virtual void PhysWalkRestricted(float deltaTime, int32 Iterations);
	virtual void PhysCrouchRestricted(float deltaTime, int32 Iterations);

public:
	virtual float GetMaxSpeed() const override;
	virtual bool CanAttemptJump() const override;

	void SetWantsToSprint(bool bValue);
	void SetWantsToRestrict(bool bValue);

	virtual bool IsMovingOnGround() const override; 
	virtual bool IsSprinting() const;
	virtual bool IsRestrictedWalking() const;
	virtual bool IsRestrictedCrouching() const;
		
};
