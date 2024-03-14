#include "InventoryMenu.h"
#include "InventorySlot.h"
#include "ProjectCalm/Inventory/ItemDetails.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"

// #include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"


UInventoryMenu::UInventoryMenu(const FObjectInitializer &ObjectInitializer) : UPopupMenu(ObjectInitializer)
{
}

UInventorySlot* UInventoryMenu::GetHoveredSlot()
{
    for (UInventorySlot* InventorySlot : Slots) {if (InventorySlot->GetIsHovered()) {return InventorySlot;}}
    return nullptr;
}

UInventorySlot* UInventoryMenu::GetSelectedSlot()
{
    for (UInventorySlot* InventorySlot : Slots) {if (InventorySlot->GetIsSelected()) {return InventorySlot;}}
    return nullptr;
}

void UInventoryMenu::UpdateContents()
{
    UE_LOG(LogTemp, Display, TEXT("InventoryMenu:: UpdateContents()"));
    int32 i = 0;
    for (UInventorySlot* InventorySlot : Slots)
    {
        CHECK_NULLPTR_RET(InventorySlot, LogUserWidget, "InventoryMenu:: InventorySlot is NULL!");

        if (i < PlayerInventory.Num())
        {
            InventorySlot->SetIndex(i);
            InventorySlot->UpdateContents(PlayerInventory[i]);
        }
        i++;
    }
}

void UInventoryMenu::DisplayDetails(UInventorySlot* InventorySlot)
{
    FItemDetails ItemDetails = InventorySlot->GetDetails();
    if (ItemDetails.IsValid()) {UpdateDetailsWidgets(ItemDetails);}
    else {ClearDetails();}
}

void UInventoryMenu::UpdateDetailsWidgets(FItemDetails ItemDetails)
{
    if (ItemImage != nullptr)
    {
        ItemImage->SetBrushFromTexture(ItemDetails.ItemImage);
        ItemImage->SetVisibility(ESlateVisibility::Visible);
    }
    if (ItemName != nullptr)
    {
        ItemName->SetText(FText::FromString(ItemDetails.ItemName));
        ItemName->SetVisibility(ESlateVisibility::Visible);
    }
    if (ItemDescription != nullptr)
    {
        ItemDescription->SetText(FText::FromString(ItemDetails.ItemDescription));
        ItemDescription->SetVisibility(ESlateVisibility::Visible);
    }
}

void UInventoryMenu::ClearDetails()
{
    if (ItemImage != nullptr) {ItemImage->SetVisibility(ESlateVisibility::Hidden);}
    if (ItemName != nullptr) {ItemName->SetVisibility(ESlateVisibility::Hidden);}
    if (ItemDescription != nullptr) {ItemDescription->SetVisibility(ESlateVisibility::Hidden);}
}

void UInventoryMenu::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    PCPlayerStatics::GetPlayerInventory(this, PlayerInventory);

    for (UHorizontalBox* Row : {InventoryRowTop, InventoryRowMiddle, InventoryRowBottom})
    {
        CHECK_NULLPTR_RET(Row, LogUserWidget, "InventoryMenu:: Missing row in InventoryList!");

        for (UWidget* InventorySlotWidget : Row->GetAllChildren())
        {
            UInventorySlot* InventorySlot = Cast<UInventorySlot>(InventorySlotWidget);
            
            CHECK_NULLPTR_RET(InventorySlot, LogUserWidget, "InventoryMenu:: InventorySlot is NULL!");
            InventorySlot->OnMouseButtonDown.BindUObject(this, &UInventoryMenu::OnSlotClicked);
            InventorySlot->OnMouseButtonUp.BindUObject(this, &UInventoryMenu::OnSlotReleased);
            InventorySlot->OnRightMouseButtonDown.BindUObject(this, &UInventoryMenu::OnSlotRightClicked);
            Slots.Add(InventorySlot);
        }
    }

    UpdateContents();
}

void UInventoryMenu::OnSlotClicked()
{
    // UE_LOG(LogTemp, Display, TEXT("InventoryMenu:: LMB DOWN ------------------------------------------------------"));
    if (UInventorySlot* HoveredSlot = GetHoveredSlot())
    {
        // UE_LOG(LogTemp, Display, TEXT("InventoryMenu:: HoveredSlot: %i"), HoveredSlot->GetIndex());
        if (UInventorySlot* SelectedSlot = GetSelectedSlot()) {SelectedSlot->SetIsSelected(false);}
        if (!HoveredSlot->IsEmpty()) {HoveredSlot->SetIsSelected(true);}
        DisplayDetails(HoveredSlot);
    }
}

void UInventoryMenu::OnSlotReleased()
{
    // UE_LOG(LogTemp, Display, TEXT("InventoryMenu:: LMB UP ------------------------------------------------------"));
    if (UInventorySlot* HoveredSlot = GetHoveredSlot())
    {
        // UE_LOG(LogTemp, Display, TEXT("InventoryMenu:: HoveredSlot: %i"), HoveredSlot->GetIndex());
        if (UInventorySlot* SelectedSlot = GetSelectedSlot())
        {
            // UE_LOG(LogTemp, Display, TEXT("InventoryMenu:: SelectedSlot: %i"), SelectedSlot->GetIndex());
            if (HoveredSlot != SelectedSlot)
            {
                APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this);
                CHECK_NULLPTR_RET(PlayerCharacter, LogUserWidget, "InventoryMenu:: PlayerCharacter not found!");

                int32 SelectedSlotIdx = SelectedSlot->GetIndex();
                int32 HoveredSlotIdx = HoveredSlot->GetIndex();
                PlayerCharacter->SwapInventoryItems(SelectedSlotIdx, HoveredSlotIdx);

                PCPlayerStatics::GetPlayerInventory(this, PlayerInventory);
                SelectedSlot->UpdateContents(PlayerInventory[SelectedSlotIdx]);
                HoveredSlot->UpdateContents(PlayerInventory[HoveredSlotIdx]);
            }
        }
    }
}

void UInventoryMenu::OnSlotRightClicked()
{
    FString FailureResponse;
    bool bSuccess = false;
    if (UInventorySlot* HoveredSlot = GetHoveredSlot())
    {
        DisplayDetails(HoveredSlot);
        bSuccess = HoveredSlot->Activate(FailureResponse);
    }

    if (bSuccess) {UpdateContents();}
    else if (!FailureResponse.IsEmpty())
    {
        APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this);
        CHECK_NULLPTR_RET(PlayerCharacter, LogUserWidget, "InventoryMenu:: PlayerCharacter not found!");
        PlayerCharacter->NotifyPlayer(FailureResponse);
    }
}
