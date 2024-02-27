// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NotificationComponent.generated.h"

class UNotificationWidget;
class ACharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UNotificationComponent : public UActorComponent
{
	GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UNotificationWidget> WidgetClass;
    UPROPERTY(EditDefaultsOnly)
    float DisplayTime{4.0f};
    UPROPERTY(EditDefaultsOnly)
    float FadeTime{1.0f};

    UNotificationWidget* ActiveWidget{nullptr};
    float StartTimeStamp{-100.0f};

public:
	UNotificationComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    TSubclassOf<UNotificationWidget> GetWidgetClass() {return WidgetClass;};
    void SetActiveWidget(UNotificationWidget* InWidget) {ActiveWidget = InWidget;};
    void DisplayString(FString InString);
		
};
