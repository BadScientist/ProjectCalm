// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextTreeNodeWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UTextTreeNodeWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextDisplay;

	UPROPERTY(meta = (BindWidget))
	UImage* ExpandIcon;

    TArray<UTextTreeNodeWidget*> Children;

	bool bIsExpanded{false};

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) override;

private:
	void CollapseChildren();
	void ExpandChildren();

public:
    bool IsLeaf() {return Children.IsEmpty();}

	void Setup();
	void SetText(FString InString);
	void SetTextColor(FColor InColor);
	void ToggleStrikethrough();
	void AddChild(UTextTreeNodeWidget* Child);
	void AddChildren(TArray<UTextTreeNodeWidget*> ChildArray);
	void RemoveChild(UTextTreeNodeWidget* Child);

	void Expand();
	void Collapse();
	
};
