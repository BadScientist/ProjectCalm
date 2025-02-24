// Copyright 2025 Joseph D Tong aka "BadScientist"

#include "DialogueBox.h"
#include "Dialogue.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"

#include "Components/Image.h"

UDialogueBox::UDialogueBox(const FObjectInitializer& ObjectInitializer) : UFancyTextDisplay(ObjectInitializer)
{
}

void UDialogueBox::SetDialogue(FDialogue InDialogue)
{
    if (Portrait != nullptr && InDialogue.Portrait != nullptr) {Portrait->SetBrushFromTexture(InDialogue.Portrait);}

    TArray<FString> ConstructedStrings;
    for (FString InString : InDialogue.Strings)
    {
        ConstructedStrings.Add("{header}" + InDialogue.Speaker.ToString() + ":{/header}\n" + InString);
    }

    SetStringsToDisplay(ConstructedStrings);
}

void UDialogueBox::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    PCGameStatics::PauseGame(this);
}

void UDialogueBox::Teardown()
{
    PCGameStatics::UnpauseGame(this);

    Super::Teardown();
}
