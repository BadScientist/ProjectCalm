// Fill out your copyright notice in the Description page of Project Settings.

#include "FancyText.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/TextBlock.h"

UFancyText::UFancyText(const FObjectInitializer &ObjectInitializer) : UUserWidget(ObjectInitializer)
{
    SetVisibility(ESlateVisibility::Hidden);
    SetIsFocusable(false);
}

void UFancyText::SetText(FString InString)
{
    TextBlock->SetText(FText::FromString(InString));
}

void UFancyText::SetText(FText InText)
{
    TextBlock->SetText(InText);
}

FText UFancyText::GetText()
{
    CHECK_NULLPTR_RETVAL(TextBlock, LogUserWidget, "UFancyText:: No TextBlock found!", FText());
    return TextBlock->GetText();
}

FVector2D UFancyText::GetSize()
{
    CHECK_NULLPTR_RETVAL(TextBlock, LogUserWidget, "UFancyText:: No TextBlock found!", FVector2D());
    return TextBlock->GetDesiredSize();
}

void UFancyText::Print()
{
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    bReadyToMerge = true;
    OnPrint();
}
