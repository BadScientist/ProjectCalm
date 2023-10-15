// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoDataCollectorComponent.h"
#include "PhotoSubjectDataComponent.h"
#include "PhotoData.h"
#include "SpawnerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
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

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter == nullptr) {return;}

	TSubclassOf<USpawnerComponent> SpawnerClass = USpawnerComponent::StaticClass();
	Spawner = Cast<USpawnerComponent>(PlayerCharacter->GetComponentByClass(SpawnerClass));
	
}

FPhotoData UPhotoDataCollectorComponent::CollectPhotoData(FConvexVolume ViewFrustum, FVector ViewLocation)
{
	FPhotoData Data;
	Data.TimeTaken = FDateTime::Now();
	
	TArray<AActor*> PhotoSubjects;
	if (Spawner != nullptr) {PhotoSubjects = Spawner->GetAllPhotoSubjects();}
	for (AActor* Actor : PhotoSubjects)
	{
		if (Actor == nullptr) {continue;}
		if (UPhotoSubjectDataComponent* PhotoSubjectDataComponent = Actor->FindComponentByClass<UPhotoSubjectDataComponent>())
		{
			FPhotoSubjectData SubjectData;
			bool Visible = PhotoSubjectDataComponent->GeneratePhotoSubjectData(ViewFrustum, ViewLocation, SubjectData);
			if (Visible) {Data.Subjects.Add(SubjectData);}
		}
	}

    return Data;
}
