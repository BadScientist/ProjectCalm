// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoShack.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "Proprietor.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif


// Sets default values
APhotoShack::APhotoShack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    InteractionLabel = FString("Knock");

    Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    SetRootComponent(Arrow);

    ShackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShackMesh"));
    if (ShackMesh != nullptr)
    {
        ShackMesh->SetupAttachment(Arrow);
        SetupMeshCollisionAndPhysics(ShackMesh);
    }

    if (InteractionMesh != nullptr)
    {
        InteractionMesh->SetupAttachment(ShackMesh);
        SetupMeshCollisionAndPhysics(InteractionMesh);
    }

    if (InteractionCollision != nullptr)
    {
        InteractionCollision->SetupAttachment(InteractionMesh);
        SetCollisionProfile("OverlapAllDynamic");
        InteractionCollision->SetEnableGravity(false);
    }

    PlanksMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanksMesh"));
    if (PlanksMesh != nullptr)
    {
        PlanksMesh->SetupAttachment(ShackMesh);
        SetupMeshCollisionAndPhysics(PlanksMesh);
    }

    ProprietorPath.Add(FWidgetVector::Zero());
}

// Called when the game starts or when spawned
void APhotoShack::BeginPlay()
{
	Super::BeginPlay();

    for (FWidgetVector Offset : ProprietorPath)
    {
        ProprietorPathLocations.Add(GetActorLocation() + Offset.Vector.RotateAngleAxis(GetActorRotation().Yaw, FVector::UpVector));
    }

    if (InteractionMesh != nullptr) {WindowDefaultPosition = InteractionMesh->GetRelativeLocation();}
}

void APhotoShack::Interact(APlayerCharacter* InteractingPlayer)
{
    Super::Interact(InteractingPlayer);

    SetState(EShackState::PROPRIETOR_MOVING_IN);
}

// Called every frame
void APhotoShack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (ShackState == EShackState::SHOP_OPEN && \
        PlayerCharacter != nullptr && \
        FVector::Distance(GetActorLocation(), PlayerCharacter->GetActorLocation()) > 1500) 
    {
        SetState(EShackState::PROPRIETOR_MOVING_OUT);
    }

    if (ShackState == EShackState::PROPRIETOR_MOVING_IN || ShackState == EShackState::PROPRIETOR_MOVING_OUT) {MoveProprietor(DeltaTime);}

    if (ShackState == EShackState::WINDOW_OPENING || ShackState == EShackState::WINDOW_CLOSING) {MoveWindow(DeltaTime);}

}

void APhotoShack::SetupMeshCollisionAndPhysics(UStaticMeshComponent* Mesh)
{
    CHECK_NULLPTR_RET(Mesh, LogInteractable, "APhotoShack:: SetupMeshCollisionAndPhysics was passed a nullptr!");
    Mesh->SetCollisionProfileName("BlockAllDynamic");
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetEnableGravity(false);
}

void APhotoShack::SetState(EShackState NewState)
{
    EShackState PreviousState = ShackState;
    ShackState = NewState;

    CHECK_NULLPTR_RET(InteractionCollision, LogInteractable, "PhotoShack:: InteractionCollision not found!");
    CHECK_NULLPTR_RET(Proprietor, LogInteractable, "PhotoShack:: Proprietor not found!");

    if (PreviousState != ShackState)
    {
        switch (ShackState)
        {
        case EShackState::SHOP_CLOSED:
            SetCollisionEnabled(true);
            break;
        case EShackState::PROPRIETOR_MOVING_IN:
            SetCollisionEnabled(false);
            ProprietorPathIdx = 1;

#ifdef LOCAL_DEBUG_LOGS
            {FString LocationsString;
            for (int32 i = 0; i < ProprietorPathLocations.Num(); i++) 
            {
                LocationsString.Append(ProprietorPathLocations[i].ToCompactString() + (i == ProprietorPathIdx ? "* " : " "));
            }
            UE_LOG(LogInteractable, Display, TEXT("PhotoShack:: ProprietorPath: %s"), *LocationsString);}
#endif

            break;
        case EShackState::PROPRIETOR_MOVING_OUT:
            ProprietorPathIdx = ProprietorPathLocations.Num() - 2;

#ifdef LOCAL_DEBUG_LOGS
            {FString LocationsString;
            for (int32 i = 0; i < ProprietorPathLocations.Num(); i++) 
            {
                LocationsString.Append(ProprietorPathLocations[i].ToCompactString() + (i == ProprietorPathIdx ? "* " : " "));
            }
            UE_LOG(LogInteractable, Display, TEXT("PhotoShack:: ProprietorPath: %s"), *LocationsString);}
#endif

            break;
        case EShackState::SHOP_OPEN:
            if (Proprietor != nullptr){Proprietor->SetCollisionEnabled(true);}
            break;
        case EShackState::WINDOW_CLOSING:
            if (Proprietor != nullptr){Proprietor->SetCollisionEnabled(false);}
            PlayWindowMoveSound();
            break;
        case EShackState::WINDOW_OPENING:
            PlayWindowMoveSound();
            break;
        
        default:
            break;
        }
    }
}

void APhotoShack::MoveProprietor(float DeltaTime)
{
    CHECK_NULLPTR_RET(Proprietor, LogInteractable, "PhotoShack:: No Proprietor found!");
    if (ProprietorPathLocations.Num() < 2) {return;}

    FVector CurrentPosition = Proprietor->GetActorLocation();

    FVector StartLocation = ShackState == EShackState::PROPRIETOR_MOVING_IN ? ProprietorPathLocations[ProprietorPathIdx - 1] : ProprietorPathLocations[ProprietorPathIdx + 1];
    FVector Destination = ProprietorPathLocations[ProprietorPathIdx];

    FVector Direction = (Destination - CurrentPosition).GetSafeNormal();
    FVector NewPosition = CurrentPosition + (Direction * ProprietorMoveSpeed * DeltaTime);

    if (FVector::Distance(NewPosition, StartLocation) >= FVector::Distance(Destination, StartLocation))
    {
        Proprietor->SetActorLocation(Destination);

        if (ShackState == EShackState::PROPRIETOR_MOVING_IN)
        {
            if (ProprietorPathIdx == ProprietorPathLocations.Num() - 1) {SetState(EShackState::WINDOW_OPENING);}
            else {ProprietorPathIdx++;}
        }
        else
        {
            if (ProprietorPathIdx == 0) {SetState(EShackState::WINDOW_CLOSING);}
            else {ProprietorPathIdx--;}
        }
    }
    else {Proprietor->SetActorLocation(NewPosition);}
}

void APhotoShack::MoveWindow(float DeltaTime)
{
    CHECK_NULLPTR_RET(InteractionMesh, LogInteractable, "PhotoShack:: WindowMesh not found!");

    WindowProgress += (DeltaTime / WindowSlideTime) * (ShackState == EShackState::WINDOW_OPENING ? 1 : ShackState == EShackState::WINDOW_CLOSING ? -1 : 0);
    WindowProgress = FMath::Clamp(WindowProgress, 0, 1);

#ifdef LOCAL_DEBUG_LOGS
    UE_LOG(LogTemp, Display, TEXT("ShackState: %u, WindowProgress: %f"), (uint8)ShackState, WindowProgress);
#endif

    float VerticalOffset = WindowOpenZOffset * (WindowProgress < 0.5 ? (4 * FMath::Pow(WindowProgress, 3)) : (1 - FMath::Pow(-2 * WindowProgress + 2, 3) / 2));
    InteractionMesh->SetRelativeLocation(FVector(WindowDefaultPosition.X, WindowDefaultPosition.Y, WindowDefaultPosition.Z + VerticalOffset));

    if (ShackState == EShackState::WINDOW_OPENING && FMath::IsNearlyEqual(WindowProgress, 1))
    {
        SetState(EShackState::SHOP_OPEN);
    }
    else if (ShackState == EShackState::WINDOW_CLOSING && FMath::IsNearlyZero(WindowProgress))
    {
        SetState(EShackState::SHOP_CLOSED);
    }
}

void APhotoShack::PlayWindowMoveSound()
{
    if (!WindowMoveSound.IsNone())
    {
        UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
        CHECK_NULLPTR_RET(GameInstance, LogInteractable, "PhotoShack:: No Game Instance found!");
        GameInstance->PlayDiageticSound(WindowMoveSound, this, GetActorLocation());
    }
}
