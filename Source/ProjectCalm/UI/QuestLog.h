// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PopupMenu.h"
#include "QuestLog.generated.h"

class UVerticalBox;
class UTextTreeNodeWidget;
struct FQuestDetails;
struct FStageDetails;
struct FObjectiveDetails;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UQuestLog : public UPopupMenu
{
	GENERATED_BODY()
	

private:
	UPROPERTY(meta = (BindWidget))
    UVerticalBox* TaskList;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UTextTreeNodeWidget> RootWidgetClass;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UTextTreeNodeWidget> BranchWidgetClass;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UTextTreeNodeWidget> LeafWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	FColor CompletedColor{FColor::Silver};

    UPROPERTY(EditDefaultsOnly)
	float Indent{64.0f};

public:
	virtual void Setup(bool bIsInteractiveIn = true) override;

private:
	void CreateQuestEntry(FQuestDetails Quest);
	void CreateStageEntry(FStageDetails Stage, UTextTreeNodeWidget* Parent);
	void CreateObjectiveEntry(FObjectiveDetails Objective, UTextTreeNodeWidget* Parent);
	
};
