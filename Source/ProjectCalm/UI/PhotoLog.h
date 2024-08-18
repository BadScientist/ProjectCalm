// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PopupMenu.h"
#include "PhotoLog.generated.h"

class UImage;
class UBorder;
class UButton;
struct FPhotoData;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UPhotoLog : public UPopupMenu
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
    UImage* PhotoDisplay;
	UPROPERTY(meta = (BindWidget))
    UBorder* PhotoDisplayBorder;
	UPROPERTY(meta = (BindWidget))
    UButton* LeftButton;
	UPROPERTY(meta = (BindWidget))
    UButton* RightButton;
	UPROPERTY(meta = (BindWidget))
    UButton* DeleteButton;

	uint32 CameraID{0};
    TArray<FPhotoData> Photos;
	int32 PhotoIdx{0};

protected:
	virtual bool Initialize() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	void UpdateDisplay();

	UFUNCTION()
	void NextPhoto();
	UFUNCTION()
	void PreviousPhoto();
	UFUNCTION()
	void DeletePhoto();

public:
	void PopulateLog(uint32 InCameraID);
	
};
