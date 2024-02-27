#include "InteractionComponent.h"
#include "ProjectCalm/Gameplay/InteractableInterface.h"
#include "PlayerCharacter.h"

#include "Components/TextBlock.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    FText LabelText;

    FHitResult OutHit;
    if (GetFirstInteractableInRange(OutHit))
    {
        TargetedInteractable = Cast<IInteractableInterface>(OutHit.GetActor());
        if (TargetedInteractable != nullptr) {LabelText = FText::FromString(TargetedInteractable->GetInteractionLabel());}
        else {LabelText = FText::GetEmpty();}
    }
    else {LabelText = FText::GetEmpty();}

    if (InteractionLabelText != nullptr) {InteractionLabelText->SetText(LabelText);}
}

void UInteractionComponent::Interact()
{
    if (TargetedInteractable == nullptr) {return;}
    TargetedInteractable->Interact(Cast<APlayerCharacter>(GetOwner()));
}

bool UInteractionComponent::GetFirstInteractableInRange(FHitResult& OutHit) const
{
	UWorld* World = GetWorld();
	FVector Start = GetComponentLocation();
	FVector End = GetForwardVector() * MaxDistance + Start;
	FQuat Rotation = FQuat::Identity;
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
	
	return World->SweepSingleByChannel(OutHit, Start, End, Rotation, TraceChannel, CollisionShape);
}

bool UInteractionComponent::GetTargetedInteractableLabel(FString& OutString)
{
    if (TargetedInteractable == nullptr) {return false;}

    OutString = TargetedInteractable->GetInteractionLabel();
    return true;
}
