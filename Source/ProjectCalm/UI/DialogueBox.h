// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FancyTextDisplay.h"
#include "DialogueBox.generated.h"

class UImage;
struct FDialogue;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UDialogueBox : public UFancyTextDisplay
{
	GENERATED_BODY()
    
	UPROPERTY(meta = (BindWidget))
    UImage* Portrait;

public:
	UDialogueBox(const FObjectInitializer& ObjectInitializer);

public:
    void SetDialogue(FDialogue InDialogue);
	
};
