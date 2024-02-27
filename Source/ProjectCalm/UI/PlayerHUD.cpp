// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "NotificationWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"


void UPlayerHUD::Hide()
{
    if (Reticle != nullptr) {Reticle->SetVisibility(ESlateVisibility::Hidden);}
    if (InteractionLabelText != nullptr) {InteractionLabelText->SetVisibility(ESlateVisibility::Hidden);}
}

void UPlayerHUD::Show()
{
    if (Reticle != nullptr) {Reticle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);}
    if (InteractionLabelText != nullptr) {InteractionLabelText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);}
}

void UPlayerHUD::SetInteractionLabelText(FString InString)
{
    if (InteractionLabelText != nullptr) {InteractionLabelText->SetText(FText::FromString(InString));}
}
