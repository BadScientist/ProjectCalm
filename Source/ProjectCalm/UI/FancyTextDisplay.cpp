#include "FancyTextDisplay.h"
#include "TextStyleData.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/SizeBox.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

#define	STYLE_TAG_START	'{'
#define	STYLE_TAG_END	'}'
#define WORD_DELIMITER	' '
#define NEWLINE_CHAR	'\n'

/*@todo: 
	- Call to ProjectCalmGameInstance from Proprietor
	- TEST
*/

UFancyTextDisplay::UFancyTextDisplay(const FObjectInitializer &ObjectInitializer) : UMenu(ObjectInitializer)
{
	// ...
}

bool UFancyTextDisplay::Initialize()
{
	bool bResult = Super::Initialize();

	if (StyleData != nullptr) {CurrentStyle = StyleData->GetDefaultStyle();}
	else {bResult = false;}

    return bResult;
}

void UFancyTextDisplay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// if (bPopulateWrapBox) {DisplayNextString();}
	if (bPrinting) {PrintNextCharacter();}	
}

FReply UFancyTextDisplay::NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent)
{
	FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	
	if (InKeyEvent.GetKey() == EKeys::E)
	{
		OnEKeyDown();
		
		if (!bInputOnCooldown)
		{
			bInputOnCooldown = true;
			GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UFancyTextDisplay::ClearCooldown, InputCooldown);

			if (bPrinting) {CompleteCurrentString();}
			else {DisplayNextString();}
		}

		return FReply::Handled();
	}

    return Reply;
}

void UFancyTextDisplay::SetStringsToDisplay(TArray<FString> InStringsToDisplay)
{	
	StringsToDisplay.Empty();
	StringsToDisplay.Append(InStringsToDisplay);
	CurrentStringIdx = 0;
	CurrentCharacterIdx = 0;

	CHECK_NULLPTR_RET(StyleData, LogUserWidget, "FancyTextDisplay:: No StyleData found!");
	CurrentStyle = StyleData->GetDefaultStyle();

	DisplayNextString();
}

void UFancyTextDisplay::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    if (!bIsInteractive) {return;}

    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogUserWidget, "FancyTextDisplay:: PlayerController is NULL!");

    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
    InputModeData.SetWidgetToFocus(TakeWidget());
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = false;
}

void UFancyTextDisplay::Teardown()
{
    if (bIsInteractive) {SetGameOnlyControls();}

    Super::Teardown();
}

int32 UFancyTextDisplay::ReadAndSetStyle(FString InString, int32 Idx)
{
	Idx++;

	FString StyleName = FString();
	while (Idx <= InString.Len() && InString[Idx] != STYLE_TAG_END)
	{
		StyleName.AppendChar(InString[Idx]);
		Idx++;
	}
	Idx++;

	CHECK_NULLPTR_RETVAL(StyleData, LogUserWidget, "FancyTextDisplay:: No StyleData found!", Idx);
	CurrentStyle = StyleData->GetStyleByString(StyleName);
	return Idx;
}

void UFancyTextDisplay::SetupWrapBoxSlot(UWrapBoxSlot *InSlot)
{
	InSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
	InSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
	InSlot->SetPadding(FMargin(0.0f));
}

float UFancyTextDisplay::GetTotalSlotWidth(TArray<UWrapBoxSlot*> Letters)
{
	float Width{0.0f};
	for (UWrapBoxSlot* Letter : Letters)
	{
		if (Letter != nullptr && Letter->Content != nullptr)
		{
			Width += Letter->Content->GetDesiredSize().X;
		}
	}
	return Width;
}

float UFancyTextDisplay::GetMaxSlotHeight(TArray<UWrapBoxSlot*> Letters)
{
	float Height{0.0f};
	for (UWrapBoxSlot* Letter : Letters)
	{
		if (Letter != nullptr && Letter->Content != nullptr)
		{
			Height = FMath::Max(Letter->Content->GetDesiredSize().Y, Height);
		}
	}
	return Height;
}

void UFancyTextDisplay::DisplayNextString()
{
	// UE_LOG(LogUserWidget, Error, TEXT("UFancyTextDisplay::DisplayNextString()"));
	CHECK_NULLPTR_RET(TextArea, LogUserWidget, "UFancyTextDisplay:: No bound SizeBox for TextArea!");
	CHECK_NULLPTR_RET(WrapBox, LogUserWidget, "UFancyTextDisplay:: No bound WrapBox!");

	WrapBox->ClearChildren();
	
	if (CurrentStringIdx >= StringsToDisplay.Num()) {return Teardown();}
	FString CurrentString = StringsToDisplay[CurrentStringIdx];

	ForceLayoutPrepass();
	FVector2D TextAreaSize = TextArea->GetDesiredSize();
	float TotalHeight{0.0f}, LineHeight{0.0f}, LineWidth{0.0f};
	int32 LastSpace{-2}, SlotIdx{0};
	TArray<UWrapBoxSlot*> CurrentWord, CurrentLine;

	// UE_LOG(LogUserWidget, Warning, TEXT("FancyTextDisplay::TextArea Size: %s"), *TextAreaSize.ToString());

	for (int32 i = CurrentCharacterIdx; i < CurrentString.Len(); i++)
	{
		while (i < CurrentString.Len() && CurrentString[i] == STYLE_TAG_START)
		{
			i = ReadAndSetStyle(CurrentString, i);
		}
		CurrentCharacterIdx = i;
		if (i >= CurrentString.Len()) {break;}
		if (CurrentString[i] == WORD_DELIMITER) {LastSpace = i;}

		UFancyText* NewFancyText{nullptr};
		UWrapBoxSlot* NewSlot{nullptr};

		if (!(CurrentString[i] == WORD_DELIMITER && LineWidth >= TextAreaSize.X))
		{
			NewFancyText = CreateWidget<UFancyText>(this, CurrentStyle.WidgetClass);
		}

		if (NewFancyText != nullptr)
		{
			if (CurrentString[i] != NEWLINE_CHAR) {NewFancyText->SetText(CurrentString.Mid(i, 1));}
			else {NewFancyText->SetText(FString());}

			NewSlot = WrapBox->AddChildToWrapBox(NewFancyText);

			if (NewSlot != nullptr)
			{
				NewFancyText->SetCharacterIndex(SlotIdx);
				SlotIdx++;
				
				SetupWrapBoxSlot(NewSlot);
				ForceLayoutPrepass();
				FVector2D CharacterSize = NewFancyText->GetDesiredSize();
				// UE_LOG(LogUserWidget, Warning, TEXT("FancyTextDisplay::FancyText Size: %s"), *CharacterSize.ToString());
				LineWidth += CharacterSize.X;

				if (CurrentString[i] == NEWLINE_CHAR)
				{
					NewSlot->SetNewLine(true);
					LineWidth = 0.0f;
					LineHeight = 0.0f;
					CurrentWord.Empty();
					CurrentLine.Empty();
				}
				else if (LastSpace == i - 1)
				{
					CurrentLine.Append(CurrentWord);
					CurrentWord.Empty();
				}

				CurrentWord.Add(NewSlot);
				LineHeight = FMath::Max(LineHeight, CharacterSize.Y);
				// UE_LOG(LogUserWidget, Display, TEXT("FancyTextDisplay::LineHeight: %f"), LineHeight);
			}
		}

		if (LineWidth > TextAreaSize.X)
		{
			if (!CurrentWord.IsEmpty() && CurrentWord[0] != nullptr)
			{
				CurrentWord[0]->SetNewLine(true);
				for (UWrapBoxSlot* Letter : CurrentWord) {CurrentLine.Remove(Letter);}
				TotalHeight += GetMaxSlotHeight(CurrentLine);
				CurrentLine.Empty();
				CurrentLine.Append(CurrentWord);
				LineHeight = GetMaxSlotHeight(CurrentWord);				
				LineWidth = GetTotalSlotWidth(CurrentWord);
			}

			if (TotalHeight + LineHeight >= TextAreaSize.Y)
			{
				// UE_LOG(LogUserWidget, Display, TEXT("FancyTextDisplay::TotalHeight: %f, LineHeight: %f, WrapBoxHeight: %f"),
				// 	TotalHeight, LineHeight, TextAreaSize.Y);
				CurrentCharacterIdx -= (CurrentWord.Num() - 1);
				for (UWrapBoxSlot* Letter : CurrentWord) {WrapBox->RemoveChild(Letter->Content);}
				break;
			}
		}
	}

	if (CurrentCharacterIdx >= CurrentString.Len() - 1)
	{
		CurrentStringIdx++;
		CurrentCharacterIdx = 0;
	}

	PrintIdx = 0;
	bPrinting = true;
	bPopulateWrapBox = false;
}

void UFancyTextDisplay::CompleteCurrentString()
{
	while (UFancyText* FancyText = Cast<UFancyText>(WrapBox->GetChildAt(PrintIdx))) {PrintCharacter(FancyText);}
	EndPrint();
}

void UFancyTextDisplay::PrintNextCharacter()
{	
	float TimeSinceLastChar = GetWorld()->GetTimeSeconds() - LastCharacterTimeStamp;	
	if (TimeSinceLastChar < DelayPerCharacter) {return;}

	if (UFancyText* FancyText = Cast<UFancyText>(WrapBox->GetChildAt(PrintIdx)))
	{
		PrintCharacter(FancyText);
		LastCharacterTimeStamp = GetWorld()->GetTimeSeconds();
	}
	else {EndPrint();}
}

void UFancyTextDisplay::PrintCharacter(UFancyText *FancyText)
{
	FancyText->Print();
	PrintIdx++;
}

void UFancyTextDisplay::EndPrint()
{
	bPrinting = false;
	PrintIdx = 0;
}
