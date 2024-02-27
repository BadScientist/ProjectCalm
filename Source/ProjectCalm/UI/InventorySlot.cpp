// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "ProjectCalm/Inventory/ItemData.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"

DEFINE_LOG_CATEGORY(LogInventorySlot)


bool UInventorySlot::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}
    SetVisibility(ESlateVisibility::Visible);

    CHECK_NULLPTR_RETVAL(SlotButton, LogInventorySlot, "PopupMenu:: No SlotButton in Widget Blueprint!", false);
    SlotButton->SetVisibility(ESlateVisibility::HitTestInvisible);

    CHECK_NULLPTR_RETVAL(ItemIcon, LogInventorySlot, "PopupMenu:: No ItemIcon in Widget Blueprint!", false);
    ItemIcon->SetVisibility(ESlateVisibility::Hidden);

    CHECK_NULLPTR_RETVAL(EquippedIcon, LogInventorySlot, "PopupMenu:: No EquippedIcon in Widget Blueprint!", false);
    EquippedIcon->SetVisibility(ESlateVisibility::Hidden);

    return true;
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
    // UE_LOG(LogTemp, Display, TEXT("InventorySlot:: Slot %i:: OnMouseButtonDown()"), InventoryIndex);
    if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {OnMouseButtonDown.ExecuteIfBound();}
    }
    else if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {OnRightMouseButtonDown.ExecuteIfBound();}
    }

    return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

FReply UInventorySlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
    // UE_LOG(LogTemp, Display, TEXT("InventorySlot:: Slot %i:: OnMouseButtonUp()"), InventoryIndex);
    if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        OnMouseButtonUp.ExecuteIfBound();
    }

    return Super::NativeOnMouseButtonUp(InGeometry, MouseEvent);
}

void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
    bIsHovered = true;
    // UE_LOG(LogTemp, Display, TEXT("InventorySlot:: HoveredSlot: %i"), InventoryIndex);

    Super::NativeOnMouseEnter(InGeometry, MouseEvent);
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
    bIsHovered = false;

    Super::NativeOnMouseLeave(MouseEvent);
}

void UInventorySlot::NativeOnDragEnter(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
    bIsHovered = true;
    // UE_LOG(LogTemp, Display, TEXT("InventorySlot:: HoveredSlot: %i"), InventoryIndex);

    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
}

void UInventorySlot::NativeOnDragLeave(const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
    bIsHovered = false;

    Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

bool UInventorySlot::NativeOnDrop(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
    // UE_LOG(LogTemp, Display, TEXT("InventorySlot:: Slot: %i::OnDrop()"), InventoryIndex);
    OnMouseButtonUp.ExecuteIfBound();
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventorySlot::NativeOnDragCancelled(const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
    // UE_LOG(LogTemp, Display, TEXT("InventorySlot:: Slot: %i::OnDragCancelled()"), InventoryIndex);
    OnMouseButtonUp.ExecuteIfBound();
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

bool UInventorySlot::GetIsHovered()
{
    return bIsHovered;
}

void UInventorySlot::UpdateContents()
{
    CHECK_NULLPTR_RET(ItemIcon, LogInventorySlot, "InventorySlot:: No Image Widget for ItemIcon!");
    CHECK_NULLPTR_RET(EquippedIcon, LogInventorySlot, "InventorySlot:: No Image Widget for EquippedIcon!");

    if (Item != nullptr)
    {
        UTexture2D* Texture = Item->GetInventoryIcon();

        CHECK_NULLPTR_RET(Texture, LogInventorySlot, "InventorySlot:: No Texture found in ItemData!")
        ItemIcon->SetBrushFromTexture(Texture, true);
        ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

        EquippedIcon->SetVisibility(Item->GetIsEquipped() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
    }
    else
    {
        ItemIcon->SetVisibility(ESlateVisibility::Hidden);
        EquippedIcon->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UInventorySlot::UpdateContents(UItemData *NewItem)
{
    Item = NewItem;
    UpdateContents();
}

FItemDetails UInventorySlot::GetDetails()
{
    if (Item != nullptr) {return Item->GetItemDetails();}
    return FItemDetails();
}

bool UInventorySlot::Activate(FString& OutFailureResponse)
{
    if (Item != nullptr) {return Item->Activate(GetWorld(), OutFailureResponse);}
    return false;
}
