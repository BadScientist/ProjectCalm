// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PopupMenu.h"
#include "InventoryMenu.generated.h"

class UHorizontalBox;
class UInventorySlot;
class UItemData;
class UImage;
class UTextBlock;
struct FInputActionValue;
struct FItemDetails;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UInventoryMenu : public UPopupMenu
{
	GENERATED_BODY()
	

private:
	UPROPERTY(meta = (BindWidget))
    UImage* ItemImage;
	UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemDescription;
	UPROPERTY(meta = (BindWidget))
    UHorizontalBox* InventoryRowTop;
	UPROPERTY(meta = (BindWidget))
    UHorizontalBox* InventoryRowMiddle;
	UPROPERTY(meta = (BindWidget))
    UHorizontalBox* InventoryRowBottom;

    TArray<UItemData*> PlayerInventory;
    TArray<UInventorySlot*> Slots;

	UInventorySlot* GetHoveredSlot();
	UInventorySlot* GetSelectedSlot();

public:
    void UpdateContents();
	void DisplayDetails(UInventorySlot* InventorySlot);
	void UpdateDetailsWidgets(FItemDetails ItemDetails);
	void ClearDetails();
	virtual void Setup(bool bIsInteractiveIn = true) override;
	
	void OnSlotClicked();
	void OnSlotReleased();
	void OnSlotRightClicked();
	
};
