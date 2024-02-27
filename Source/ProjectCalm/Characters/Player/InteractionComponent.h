// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"

class IInteractableInterface;
class UTextBlock;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

	UTextBlock* InteractionLabelText{nullptr};

public:	
	// Sets default values for this component's properties
	UInteractionComponent();
    
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Interact();

	void SetInteractionLabelText(UTextBlock* InTextBlock) {InteractionLabelText = InTextBlock;};

private:
	UPROPERTY(EditAnywhere)
	float MaxDistance{300};
	UPROPERTY(EditAnywhere)
	float Radius{10};

    IInteractableInterface* TargetedInteractable{nullptr};

	bool GetFirstInteractableInRange(FHitResult& OutHit) const;
    bool GetTargetedInteractableLabel(FString& OutString);
		
};
