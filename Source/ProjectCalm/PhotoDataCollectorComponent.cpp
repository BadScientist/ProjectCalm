// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoDataCollectorComponent.h"
#include "PhotoSubjectComponent.h"
#include "PhotoData.h"

#include "ConvexVolume.h"


// Sets default values for this component's properties
UPhotoDataCollectorComponent::UPhotoDataCollectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPhotoDataCollectorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FPhotoData UPhotoDataCollectorComponent::CollectPhotoData(FConvexVolume ViewFrustum, FVector ViewLocation)
{
	FPhotoData Data;
	Data.TimeTaken = FDateTime::Now();
	
	TArray<AActor*> AllActors;
	if (ULevel* Level = GetWorld()->GetCurrentLevel()) {AllActors = Level->Actors;}
	for (AActor* Actor : AllActors)
	{
		if (Actor == nullptr) {continue;}
		if (UPhotoSubjectComponent* PhotoSubjectComponent = Actor->FindComponentByClass<UPhotoSubjectComponent>())
		{
			FPhotoSubjectData SubjectData;
			bool Visible = PhotoSubjectComponent->GeneratePhotoSubjectData(ViewFrustum, ViewLocation, SubjectData);
			if (Visible) {Data.Subjects.Add(SubjectData);}
		}
	}

    return Data;
}
