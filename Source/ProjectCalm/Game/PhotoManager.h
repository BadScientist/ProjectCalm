// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhotoManager.generated.h"

struct FPhotoData;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UPhotoManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = PhotoData)
	TArray<FPhotoData> Photos;

public:
	UPhotoManager();
	
	virtual void BeginPlay() override;
	
};
