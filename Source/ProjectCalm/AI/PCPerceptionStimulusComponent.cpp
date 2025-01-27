// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPerceptionStimulusComponent.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionSystem.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif


UPCPerceptionStimulusComponent::UPCPerceptionStimulusComponent(const FObjectInitializer& ObjectInitializer) : 
    UAIPerceptionStimuliSourceComponent(ObjectInitializer)
{
    bAutoActivate = true;
    bAutoRegisterAsSource = true;
    RegisterAsSourceForSenses.Append({UAISense_Sight::StaticClass(), UAISense_Hearing::StaticClass()});
}

void UPCPerceptionStimulusComponent::OnRegister()
{
    Super::OnRegister();

#ifdef LOCAL_DEBUG_LOGS
    FString Senses = "";
    for (auto& SenseClass : RegisterAsSourceForSenses) {Senses.Append(SenseClass.Get()->GetName() + ", ");}  

    UE_LOG(LogTemp, Display, TEXT("PCPerceptionStimulusComponent:: %successfully registered to %s for %s"),
        *FString(bSuccessfullyRegistered ? "S" : "Uns"), *GetOwner()->GetActorNameOrLabel(), *Senses);
#endif  
}
