#include "PhotoLog.h"
#include "ProjectCalm/Photos/PhotoData.h"
#include "ProjectCalm/UI/TextTreeNodeWidget.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"

#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"


bool UPhotoLog::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(LeftButton, LogUserWidget, "PhotoLog:: No LeftButton in Widget Blueprint!", false);
    LeftButton->OnClicked.AddDynamic(this, &UPhotoLog::PreviousPhoto);
    LeftButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(RightButton, LogUserWidget, "PhotoLog:: No RightButton in Widget Blueprint!", false);
    RightButton->OnClicked.AddDynamic(this, &UPhotoLog::NextPhoto);
    RightButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(DeleteButton, LogUserWidget, "PhotoLog:: No DeleteButton in Widget Blueprint!", false);
    DeleteButton->OnClicked.AddDynamic(this, &UPhotoLog::DeletePhoto);
    DeleteButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);
    
    return true;
}

FReply UPhotoLog::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::A) {PreviousPhoto();}
    else if (InKeyEvent.GetKey() == EKeys::D) {NextPhoto();}
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UPhotoLog::UpdateDisplay()
{
    CHECK_NULLPTR_RET(PhotoDisplay, LogUserWidget, "PhotoLog:: No PhotoDisplay found!");
    CHECK_NULLPTR_RET(PhotoDisplayBorder, LogUserWidget, "PhotoLog:: No PhotoDisplayBorder found!");
    CHECK_NULLPTR_RET(PhotoDetails, LogUserWidget, "PhotoLog:: No PhotoDetails found!");
    CHECK_NULLPTR_RET(ScoreText, LogUserWidget, "PhotoLog:: No ScoreText found!");
    CHECK_NULLPTR_RET(SubjectsText, LogUserWidget, "PhotoLog:: No SubjectsText found!");
    
    if (Photos.IsEmpty())
    {
        PhotoDisplay->SetVisibility(ESlateVisibility::Hidden);
        PhotoDisplayBorder->SetVisibility(ESlateVisibility::Hidden);
        PhotoDetails->SetVisibility(ESlateVisibility::Hidden);
    }
    else 
    {
        // @todo: move re-usable code to photo manager
        if (Photos[PhotoIdx].Image != nullptr && Photos[PhotoIdx].Image->IsValidLowLevel())
        {
            UMaterialInstanceDynamic* PhotoRenderMat = PhotoDisplay->GetDynamicMaterial();
            CHECK_NULLPTR_RET(PhotoRenderMat, LogUserWidget, "PhotoLog:: PhotoDisplay has no DynamicMaterial!");

            PhotoRenderMat->SetTextureParameterValue(TEXT("PhotoRender"), Photos[PhotoIdx].Image);
            PhotoDisplay->SetVisibility(ESlateVisibility::Visible);
            PhotoDisplayBorder->SetVisibility(ESlateVisibility::Visible);
        }
        
        FString SubjectsString;
        for (int32 i = 0; i < Photos[PhotoIdx].Subjects.Num(); i++)
        {
            SubjectsString += PCSubjectName::EnumToString(Photos[PhotoIdx].Subjects[i].Name);
            if (Photos[PhotoIdx].Subjects.Num() - i > 1) {SubjectsString += ", ";}
        }

        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), Photos[PhotoIdx].Score)));
        SubjectsText->SetText(FText::FromString(SubjectsString));
        PhotoDetails->SetVisibility(ESlateVisibility::Visible);
    }
}

void UPhotoLog::NextPhoto()
{
    PlayButtonPressedSound();
    if (Photos.IsEmpty()) {return;}
    PhotoIdx = (PhotoIdx + 1) % Photos.Num();
    UpdateDisplay();
}

void UPhotoLog::PreviousPhoto()
{
    PlayButtonPressedSound();
    if (Photos.IsEmpty()) {return;}
    PhotoIdx = (Photos.Num() + ((PhotoIdx - 1) % Photos.Num())) % Photos.Num();
    UpdateDisplay();
}

void UPhotoLog::DeletePhoto()
{
    PlayButtonPressedSound();
    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogGameMode, "PhotoLog:: Could not find Game Mode!");
    if (GameMode->RemovePhoto(CameraID, PhotoIdx)) {Photos.RemoveAt(PhotoIdx);}

    if (PhotoIdx >= Photos.Num()) {PreviousPhoto();}
    UpdateDisplay();
}

void UPhotoLog::PopulateLog(uint32 InCameraID)
{
    CameraID = InCameraID;

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogGameMode, "PhotoLog:: Could not find Game Mode!");
    GameMode->GetPhotos(CameraID, Photos);

    UpdateDisplay();
}
