#include "DialogueBox.h"
#include "Dialogue.h"

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
        ConstructedStrings.Add("{header}" + InDialogue.Speaker.ToString() + ": {/header}" + InString);
    }

    SetStringsToDisplay(ConstructedStrings);
}
