// Fill out your copyright notice in the Description page of Project Settings.


#include "Proprietor.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif


// Sets default values
AProprietor::AProprietor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    InteractionLabel = FString("Talk");
    DisplayName = FName("Suzanne the Proprietor");
    DefaultDialogue.Add(FString("I have nothing for you. Begone!"));

    if (InteractionCollision != nullptr)
    {
        SetCollisionProfile("OverlapAllDynamic");
        InteractionCollision->SetEnableGravity(false);
    }

    if (InteractionMesh != nullptr)
    {
        InteractionMesh->SetCollisionProfileName("BlockAllDynamic");
        InteractionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        InteractionMesh->SetEnableGravity(false);
    }
}

// Called when the game starts or when spawned
void AProprietor::BeginPlay()
{
	Super::BeginPlay();
    
    SetCollisionEnabled(false);

    TargetZOffset = FMath::RandRange(MinZOffset, MaxZOffset);
    CurrentSpeed = FMath::RandRange(FMath::Max(MoveSpeed - MoveSpeedVariation, 0), MoveSpeed + MoveSpeedVariation);
}

void AProprietor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CHECK_NULLPTR_RET(InteractionMesh, LogInteractable, "Proprietor:: No Interaction Mesh!");

    FVector CurrentLocation = InteractionMesh->GetRelativeLocation();
    Progress = FMath::Clamp(Progress + DeltaTime / (FMath::Abs(TargetZOffset - LastZOffset) / CurrentSpeed), 0, 1);

    float NewZOffset = LastZOffset - 0.5f * (TargetZOffset - LastZOffset) * (FMath::Cos(PI * Progress) - 1);
    InteractionMesh->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, NewZOffset));

#ifdef LOCAL_DEBUG_LOGS
    UE_LOG(LogInteractable, Display, TEXT("Proprietor:: CurrentLocation: %s, TargetZOffset: %f, Progress: %f, Speed: %f, NewZOffset: %f"), 
        *(CurrentLocation.ToCompactString()),
        TargetZOffset,
        Progress,
        CurrentSpeed,
        NewZOffset);
#endif

    if (FMath::IsNearlyEqual(Progress, 1.0f))
    {
        LastZOffset = TargetZOffset;
        TargetZOffset = FMath::RandRange(MinZOffset, MaxZOffset) * (LastZOffset < 0 ? 1 : -1);
        CurrentSpeed = FMath::RandRange(FMath::Max(MoveSpeed - MoveSpeedVariation, 0), MoveSpeed + MoveSpeedVariation);
        Progress = 0;
    }
}

void AProprietor::Interact(APlayerCharacter* InteractingPlayer)
{
    Super::Interact(InteractingPlayer);
    
}
