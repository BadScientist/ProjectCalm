// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SoundManager.generated.h"

class UDataTable;
class UAudioComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API USoundManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
    float MasterVolumeMultiplier{0.75f};

	UPROPERTY(EditAnywhere)
    float DiageticVolumeMultiplier{0.75f};

	UPROPERTY(EditAnywhere)
    float UIVolumeMultiplier{0.75f};

	UPROPERTY(EditAnywhere)
    float MusicVolumeMultiplier{0.75f};

	UPROPERTY(EditAnywhere)
    float AmbienceVolumeMultiplier{0.75f};

	UPROPERTY(EditDefaultsOnly)
    TMap<FName, USoundBase*> SoundData;

	UPROPERTY()
    TMap<FName, UAudioComponent*> ActiveMusic;

	UPROPERTY()
    TMap<FName, UAudioComponent*> ActiveAmbience;

public:	
	// Sets default values for this component's properties
	USoundManager();
	
	virtual void BeginPlay() override;

	float GetMasterVolume() {return MasterVolumeMultiplier;};
	float GetMusicVolume() {return MusicVolumeMultiplier;};
	float GetAmbientVolume() {return AmbienceVolumeMultiplier;};
	float GetSFXVolume() {return DiageticVolumeMultiplier;};
	float GetUIVolume() {return UIVolumeMultiplier;};
	void SetMasterVolume(float InVolume);
	void SetMusicVolume(float InVolume);
	void SetAmbientVolume(float InVolume);
	void SetSFXVolume(float InVolume) {DiageticVolumeMultiplier = InVolume;};
	void SetUIVolume(float InVolume) {UIVolumeMultiplier = InVolume;};
    void PlayDiageticSound(FName SoundName, UObject* WorldContextObject, FVector SourceLocation, float VolumeMultiplier);
    void PlayUISound(FName SoundName, UObject* WorldContextObject, bool bPersistOnLevelLoad = false);
    void PlayMusicOrAmbientSound(FName SoundName, UObject* WorldContextObject, bool bIsMusic = false, bool bPersistOnLevelLoad = false);

private:
	UAudioComponent* CreateAudioComponent(FName SoundName, UObject* WorldContextObject);
	void UpdateMusicVolume();
	void UpdateAmbientVolume();
	
};
