#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "ProjectCalm/Characters/Player/PlayerCharacter.h"

class UItemData;


class PCPlayerStatics
{
public:
    static APlayerCharacter* GetPlayerCharacter(const UObject* WorldContextObject)
    {
        return Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
    }

    static APlayerController* GetPlayerController(const UObject* WorldContextObject)
    {
        APlayerCharacter* PlayerCharacter = GetPlayerCharacter(WorldContextObject);
        
        if (PlayerCharacter == nullptr) {return nullptr;}
        return Cast<APlayerController>(PlayerCharacter->GetController());
    }

    static UCameraComponent* GetPlayerCamera(const UObject* WorldContextObject)
    {
        APlayerCharacter* PlayerCharacter = GetPlayerCharacter(WorldContextObject);
        
        if (PlayerCharacter == nullptr) {return nullptr;}
        return PlayerCharacter->GetCameraComponent();
    }

    static void GetPlayerInventory(const UObject* WorldContextObject, TArray<UItemData*>& OutInventory)
    {
        APlayerCharacter* PlayerCharacter = GetPlayerCharacter(WorldContextObject);
        
        if (PlayerCharacter == nullptr) {return;}
        PlayerCharacter->GetInventory(OutInventory);
    }

    static UEnhancedInputComponent* GetEnhancedInputComponent(const UObject* WorldContextObject)
    {
        APlayerController* PlayerController = GetPlayerController(WorldContextObject);

        if (PlayerController == nullptr) {return nullptr;}
        return Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
    }

    static UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem(const UObject* WorldContextObject)
    {
        APlayerController* PlayerController = GetPlayerController(WorldContextObject);

        if (PlayerController == nullptr) {return nullptr;}
        return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    }
};