// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"

#include "TextStyle.h"
#include "Menu.h"
#include "FancyTextDisplay.generated.h"

class USizeBox;
class UWrapBox;
class UWrapBoxSlot;
class UTextStyleData;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UFancyTextDisplay : public UMenu
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	USizeBox* TextArea;
	
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox;

	UPROPERTY(EditDefaultsOnly, Category = Styles)
	UTextStyleData* StyleData;

	UPROPERTY(EditDefaultsOnly, Category = Tuning)
	float DelayPerCharacter{0.075f};

	UPROPERTY(EditDefaultsOnly, Category = Tuning)
	int32 MaxCharactersPerLine{24};

	UPROPERTY(EditDefaultsOnly, Category = Tuning)
	int32 MaxLines{6};

	UPROPERTY(EditDefaultsOnly, Category = Tuning)
	float InputCooldown{1.5f};

	UPROPERTY(VisibleAnywhere)
	TArray<FString> StringsToDisplay;

	int32 CurrentStringIdx{0};
	int32 CurrentCharacterIdx{0};
	FFancyTextStyle CurrentStyle;

	bool bPopulateWrapBox{false};
	bool bPrinting{false};
	int32 PrintIdx{0};
	float LastCharacterTimeStamp{0.0f};

	bool bInputOnCooldown{false};
	FTimerHandle CooldownTimerHandle;
	void ClearCooldown() {bInputOnCooldown = false;}

public:
	UFancyTextDisplay(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnEKeyDown();

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

private:
	int32 ReadAndSetStyle(FString InString, int32 Idx);
	void SetupWrapBoxSlot(UWrapBoxSlot* InSlot);
	float GetTotalSlotWidth(TArray<UWrapBoxSlot*> Letters);
	float GetMaxSlotHeight(TArray<UWrapBoxSlot*> Letters);
	void DisplayNextString();
	void CompleteCurrentString();
	void PrintNextCharacter();
	void PrintCharacter(UFancyText* FancyText);
	void EndPrint();

public:
	void SetStringsToDisplay(TArray<FString> InStringsToDisplay);
	
	virtual void Setup(bool bIsInteractiveIn = true) override;
    virtual void Teardown() override;
	
};
