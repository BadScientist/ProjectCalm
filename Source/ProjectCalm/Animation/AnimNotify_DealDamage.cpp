// Copyright 2024 Joseph D Tong aka "BadScientist"


#include "AnimNotify_DealDamage.h"
#include "ProjectCalm/AI/PhotoSubjectAIController.h"
#include "ProjectCalm/Characters/DamageDealerComponent.h"
#include "ProjectCalm/Characters/HealthComponent.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "GameFramework/Character.h"


void UAnimNotify_DealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    CHECK_NULLPTR_RET(MeshComp, LogAnimation, "AnimNotify_DealDamage:: Notify called with null MeshComp!");
    ACharacter* OwningCharacter = Cast<ACharacter>(MeshComp->GetOwner());

    CHECK_NULLPTR_RET(OwningCharacter, LogAnimation, "AnimNotify_DealDamage:: Failed to get OwningCharacter!");
    UDamageDealerComponent* DamageDealer = OwningCharacter->FindComponentByClass<UDamageDealerComponent>();
    APhotoSubjectAIController* OwnerController = Cast<APhotoSubjectAIController>(OwningCharacter->GetController());

    CHECK_NULLPTR_RET(OwnerController, LogAnimation, "AnimNotify_DealDamage:: Failed to get OwnerController!");
    OwnerController->ActivateAttackCooldown();
    AActor* DamageReceiver = OwnerController->GetCurrentTarget();
    
    CHECK_NULLPTR_RET(DamageDealer, LogAnimation, "AnimNotify_DealDamage:: Failed to get DamageDealerComponent!");
    DamageDealer->DealDamage(Damage, DamageReceiver, DamageMessage);
}
