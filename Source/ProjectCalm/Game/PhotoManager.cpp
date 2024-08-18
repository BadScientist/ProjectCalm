#include "PhotoManager.h"
#include "ProjectCalm/Photos/PhotoData.h"
#include "ProjectCalm/Utilities/LogMacros.h"


UPhotoManager::UPhotoManager()
{
}

void UPhotoManager::BeginPlay()
{
	Super::BeginPlay();
    
}

void UPhotoManager::GetPhotos(int32 CameraID, TArray<FPhotoData>& OutPhotos)
{
	OutPhotos.Empty();
    if (!Photos.Contains(CameraID)) {return;}

    OutPhotos.Append(Photos[CameraID].Array);
}

void UPhotoManager::GetAllPhotos(TArray<FPhotoData>& OutPhotos)
{
	OutPhotos.Empty();
	TArray<FPhotoArray> PhotoArrays;
	Photos.GenerateValueArray(PhotoArrays);

	for (FPhotoArray PhotoArray : PhotoArrays)
	{
		OutPhotos.Append(PhotoArray.Array);
	}
}

FPhotoData UPhotoManager::GetPhoto(int32 CameraID, int32 PhotoIdx)
{
	if (!Photos.Contains(CameraID) || PhotoIdx >= Photos[CameraID].Array.Num()) {return FPhotoData();}
	return Photos[CameraID].Array[PhotoIdx];
}

uint32 UPhotoManager::GetNumPhotos(int32 CameraID)
{
	if (Photos.Contains(CameraID)) {return Photos[CameraID].Array.Num();}
    return 0;
}

void UPhotoManager::AddPhoto(int32 CameraID, FPhotoData Photo)
{
	if (!Photos.Contains(CameraID)) {Photos.Add(CameraID);}
	Photos[CameraID].Array.Add(Photo);
}

bool UPhotoManager::RemovePhoto(int32 CameraID, int32 PhotoIdx)
{
	if (!Photos.Contains(CameraID)) {return false;}
	if (PhotoIdx < Photos[CameraID].Array.Num())
	{
		Photos[CameraID].Array.RemoveAt(PhotoIdx);
		return true;
	}
	return false;
}

void UPhotoManager::ClearAllPhotos(int32 CameraID)
{
	if (Photos.Contains(CameraID)) {Photos[CameraID].Array.Empty();}
}
