// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoShack.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "Proprietor.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"


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
}

// Called when the game starts or when spawned
void APhotoShack::BeginPlay()
{
	Super::BeginPlay();

    if (Proprietor != nullptr)
    {
        ProprietorInactiveLocation = Proprietor->GetActorLocation();
        ProprietorActiveLocation = GetActorLocation() + ProprietorActiveOffset.Vector.RotateAngleAxis(GetActorRotation().Yaw, FVector::UpVector);
    }

	if (InteractionMesh != nullptr) {WindowClosedPosition = InteractionMesh->GetRelativeLocation();}
    WindowOpenPosition = FVector(WindowClosedPosition.X, WindowClosedPosition.Y, WindowClosedPosition.Z + 55);
}

void APhotoShack::Interact(APlayerCharacter* InteractingPlayer)
{
    Super::Interact(InteractingPlayer);

	// @todo: Play knocking animation & sfx

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
            break;
        case EShackState::SHOP_OPEN:
            if (Proprietor != nullptr){Proprietor->SetCollisionEnabled(true);}
            break;
        case EShackState::WINDOW_CLOSING:
            if (Proprietor != nullptr){Proprietor->SetCollisionEnabled(false);}
            break;
        
        default:
            break;
        }
    }

    // FString StateString = ShackState == EShackState::SHOP_OPEN ? FString("Shop Open") : \
    //     ShackState == EShackState::PROPRIETOR_MOVING_IN ? FString("Proprietor Moving In") : \
    //     ShackState == EShackState::WINDOW_OPENING ? FString("Window Opening") : \
    //     ShackState == EShackState::SHOP_CLOSED ? FString("Shop Closed") : \
    //     ShackState == EShackState::WINDOW_CLOSING ? FString("Window Closing") : FString("Proprietor Moving Out");

    // UE_LOG(LogTemp, Warning, TEXT("PhotoShack:: State: %s"), *StateString);
}

void APhotoShack::MoveProprietor(float DeltaTime)
{
    CHECK_NULLPTR_RET(Proprietor, LogInteractable, "PhotoShack:: No Proprietor found!");

    FVector CurrentPosition = Proprietor->GetActorLocation();

    FVector StartLocation = ShackState == EShackState::PROPRIETOR_MOVING_IN ? ProprietorInactiveLocation : ProprietorActiveLocation;
    FVector Destination = ShackState == EShackState::PROPRIETOR_MOVING_IN ? ProprietorActiveLocation : ProprietorInactiveLocation;

    FVector Direction = (Destination - CurrentPosition).GetSafeNormal();
    FVector NewPosition = CurrentPosition + (Direction * ProprietorMoveSpeed * DeltaTime);

    if (FVector::Distance(NewPosition, StartLocation) >= FVector::Distance(Destination, StartLocation))
    {
        Proprietor->SetActorLocation(Destination);

        if (ShackState == EShackState::PROPRIETOR_MOVING_IN) {SetState(EShackState::WINDOW_OPENING);}
        else {SetState(EShackState::WINDOW_CLOSING);}
    }
    else {Proprietor->SetActorLocation(NewPosition);}
}

void APhotoShack::MoveWindow(float DeltaTime)
{
    CHECK_NULLPTR_RET(InteractionMesh, LogInteractable, "PhotoShack:: WindowMesh not found!");

    float MoveDistance = WindowMoveSpeed * DeltaTime;
    FVector NewPosition = InteractionMesh->GetRelativeLocation();

    if (ShackState == EShackState::WINDOW_OPENING) {NewPosition.Z = FMath::Min(NewPosition.Z + MoveDistance, WindowOpenPosition.Z);}
    else {NewPosition.Z = FMath::Max(NewPosition.Z - MoveDistance, WindowClosedPosition.Z);}
    InteractionMesh->SetRelativeLocation(NewPosition);

    if (ShackState == EShackState::WINDOW_OPENING && FMath::IsNearlyZero(WindowOpenPosition.Z - NewPosition.Z))
    {
        SetState(EShackState::SHOP_OPEN);
    }
    else if (ShackState == EShackState::WINDOW_CLOSING && FMath::IsNearlyZero(WindowClosedPosition.Z - NewPosition.Z))
    {
        SetState(EShackState::SHOP_CLOSED);
    }
}
