// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "ProjectCalm/Utilities/WidgetVector.h"

#include "PhotoShack.generated.h"

class APlayerCharacter;
class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;
class AProprietor;

UENUM()
enum EShackState
{	
    SHOP_CLOSED				=0	UMETA(DisplayName = "Shop Closed"),
	PROPRIETOR_MOVING_IN	=1	UMETA(DisplayName = "Proprietor Moving In"),
	WINDOW_OPENING			=2	UMETA(DisplayName = "Window Opening"),
	SHOP_OPEN				=3	UMETA(DisplayName = "Shop Open"),
	WINDOW_CLOSING			=4	UMETA(DisplayName = "Window Closing"),
	PROPRIETOR_MOVING_OUT	=5	UMETA(DisplayName = "Proprietor Moving Out")
};


UCLASS()
class PROJECTCALM_API APhotoShack : public AInteractableActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* Arrow{nullptr};
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* WindowCollision{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* ShackMesh{nullptr};
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* WindowMesh{nullptr};
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* PlanksMesh{nullptr};

	UPROPERTY(EditAnywhere, Category = Proprietor)
	AProprietor* Proprietor{nullptr};
	UPROPERTY(EditAnywhere, Category = Proprietor)
	float ProprietorMoveSpeed{20};
	UPROPERTY(EditAnywhere, Category = Proprietor)
	FWidgetVector ProprietorActiveOffset{FWidgetVector(0, 20, 170)};


	UPROPERTY(EditAnywhere, Category = Window)
    float WindowMoveSpeed{30};

	FVector ProprietorInactiveLocation;
	FVector ProprietorActiveLocation;
    FVector WindowClosedPosition;
    FVector WindowOpenPosition;

	EShackState ShackState{EShackState::SHOP_CLOSED};
	
public:	
	// Sets default values for this actor's properties
	APhotoShack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// START INTERACTABLEINTERFACE
    virtual void Interact(APlayerCharacter* InteractingPlayer) override;
	// END INTERACTABLEINTERFACE

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetState(EShackState NewState);
	void MoveProprietor(float DeltaTime);
	void MoveWindow(float DeltaTime);

};
