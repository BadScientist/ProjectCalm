// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UItemData;
class UImage;
struct FItemDetails;

DECLARE_DELEGATE(FMouseEventDelegate);


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

	bool bIsHovered{false};
	bool bIsSelected{false};
	UItemData* Item;
	int32 InventoryIndex{-1};

	UPROPERTY(meta = (BindWidget))
	UImage* SlotButton;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;
	UPROPERTY(meta = (BindWidget))
	UImage* EquippedIcon;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual void NativeOnDragEnter( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	virtual void NativeOnDragLeave( const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	virtual void NativeOnDragCancelled( const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;

public:
	FMouseEventDelegate OnMouseButtonDown;
	FMouseEventDelegate OnMouseButtonUp;
	FMouseEventDelegate OnRightMouseButtonDown;

	UImage* GetButton() {return SlotButton;};
	bool GetIsHovered();
	bool GetIsSelected() {return bIsSelected;};
	void SetIsSelected(bool bValue) {bIsSelected = bValue;};
	bool IsEmpty() {return Item == nullptr;};
	int32 GetIndex(){return InventoryIndex;};
	void SetIndex(int32 InIndex) {InventoryIndex = InIndex;}
	void UpdateContents();
	void UpdateContents(UItemData* NewItem);
	virtual bool Initialize() override;
	FItemDetails GetDetails();
	bool Activate(FString& OutFailureResponse);

};
