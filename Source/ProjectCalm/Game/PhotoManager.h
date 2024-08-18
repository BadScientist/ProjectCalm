// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhotoManager.generated.h"

struct FPhotoData;

USTRUCT(BlueprintType)
struct FPhotoArray
{
	GENERATED_BODY()

	FPhotoArray(){};

	UPROPERTY(VisibleAnywhere)
	TArray<FPhotoData> Array;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UPhotoManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = PhotoData)
	TMap<int32, FPhotoArray> Photos;

public:
	UPhotoManager();
	
	virtual void BeginPlay() override;

	void GetPhotos(int32 CameraID, TArray<FPhotoData>& OutPhotos);
	void GetAllPhotos(TArray<FPhotoData>& OutPhotos);
	FPhotoData GetPhoto(int32 CameraID, int32 PhotoIdx);
	uint32 GetNumPhotos(int32 CameraID);
	void AddPhoto(int32 CameraID, FPhotoData Photo);
	bool RemovePhoto(int32 CameraID, int32 PhotoIdx);
	void ClearAllPhotos(int32 CameraID);
	
};
