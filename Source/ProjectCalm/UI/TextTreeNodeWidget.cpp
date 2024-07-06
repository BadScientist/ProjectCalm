// Fill out your copyright notice in the Description page of Project Settings.

#include "TextTreeNodeWidget.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

FReply UTextTreeNodeWidget::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &MouseEvent)
{
    if (bIsExpanded)
    {
        CollapseChildren();
        bIsExpanded = false;
    }
    else
    {
        ExpandChildren();
        bIsExpanded = true;
    }

    return Super::NativeOnMouseButtonDown(InGeometry, MouseEvent);
}

void UTextTreeNodeWidget::CollapseChildren()
{
    if (IsLeaf()) {return;}

    if (ExpandIcon != nullptr) {ExpandIcon->SetRenderTransformAngle(-90.0f);}
    for (UTextTreeNodeWidget* Child : Children) 
    {
        if (Child != nullptr)
        {
            Child->CollapseChildren();
            Child->Collapse();
        }
    }
}

void UTextTreeNodeWidget::ExpandChildren()
{
    if (IsLeaf()) {return;}

    if (ExpandIcon != nullptr) {ExpandIcon->SetRenderTransformAngle(0.0f);}
    for (UTextTreeNodeWidget* Child : Children) {if (Child != nullptr) {Child->Expand();}}
}

void UTextTreeNodeWidget::Setup()
{
    CollapseChildren();
}

void UTextTreeNodeWidget::SetText(FString InString)
{
    if (TextDisplay != nullptr) {TextDisplay->SetText(FText::FromString(InString));}
}

void UTextTreeNodeWidget::SetTextColor(FColor InColor)
{    
    if (TextDisplay != nullptr) {TextDisplay->SetColorAndOpacity(FSlateColor(InColor));}
}

void UTextTreeNodeWidget::ToggleStrikethrough()
{
    if (TextDisplay != nullptr)
    {
        FSlateBrush StrikeBrush = TextDisplay->GetStrikeBrush();
        if (StrikeBrush.DrawAs == ESlateBrushDrawType::NoDrawType) {StrikeBrush.DrawAs = ESlateBrushDrawType::Image;}
        else {StrikeBrush.DrawAs = ESlateBrushDrawType::NoDrawType;}

        TextDisplay->SetStrikeBrush(StrikeBrush);
    }
}

void UTextTreeNodeWidget::AddChild(UTextTreeNodeWidget* Child)
{
    Children.Add(Child);
}

void UTextTreeNodeWidget::AddChildren(TArray<UTextTreeNodeWidget *> ChildArray)
{
    Children.Append(ChildArray);
}

void UTextTreeNodeWidget::RemoveChild(UTextTreeNodeWidget* Child)
{
    Children.Remove(Child);
}

void UTextTreeNodeWidget::Expand()
{
    SetVisibility(ESlateVisibility::Visible);
    
    if (bIsExpanded) {ExpandChildren();}
}

void UTextTreeNodeWidget::Collapse()
{
    SetVisibility(ESlateVisibility::Collapsed);
}
