// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FancyText.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UFancyText : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintGetter = GetTextBlock, meta = (BindWidget))
	UTextBlock* TextBlock;

	UPROPERTY(BlueprintGetter = GetCharacterIndex, BlueprintSetter = SetCharacterIndex)
	int32 CharacterIdx{-1};

	UPROPERTY(EditDefaultsOnly, Category = Merging)
	bool bMergeByStyle{true};

	bool bReadyToMerge{false};

public:
	UFancyText(const FObjectInitializer& ObjectInitializer);

	bool ShouldMergeByStyle() {return bMergeByStyle;}

	void SetText(FString InString);
	void SetText(FText InText);
	FText GetText();
	FVector2D GetSize();

	void Print();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPrint();

	UFUNCTION(BlueprintCallable)
	void SetReadyToMerge() {bReadyToMerge = true;}

	UFUNCTION(BlueprintPure)
	UTextBlock* GetTextBlock() {return TextBlock;}

	UFUNCTION(BlueprintPure)
	int32 GetCharacterIndex() {return CharacterIdx;}

	UFUNCTION(BlueprintCallable)
	void SetCharacterIndex(int32 InIndex) {CharacterIdx = InIndex;}
	
};
