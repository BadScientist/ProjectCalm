#include "SoundManager.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
// #include "ProjectCalm/Utilities/PCPlayerStatics.h"

#include "AudioDevice.h"
#include "Engine/DataTable.h"

USoundManager::USoundManager()
{
}

void USoundManager::BeginPlay()
{
    Super::BeginPlay();
    
}

void USoundManager::SetMasterVolume(float InVolume)
{
    MasterVolumeMultiplier = InVolume;
    UpdateMusicVolume();
    UpdateAmbientVolume();
}

void USoundManager::SetMusicVolume(float InVolume)
{
    MusicVolumeMultiplier = InVolume;
    UpdateMusicVolume();
}

void USoundManager::SetAmbientVolume(float InVolume)
{
    AmbienceVolumeMultiplier = InVolume;
    UpdateAmbientVolume();
}

void USoundManager::PlayDiageticSound(FName SoundName, UObject *WorldContextObject, FVector SourceLocation, float VolumeMultiplier)
{
    USoundBase** Sound = SoundData.Find(SoundName);
    CHECK_NULLPTR_RET(Sound, LogAudio, "USoundManager:: Invalid SoundName!");
    CHECK_NULLPTR_RET(*Sound, LogAudio, "USoundManager:: No Sound asset assigned to SoundName!");

    UGameplayStatics::PlaySoundAtLocation(WorldContextObject, *Sound, SourceLocation, MasterVolumeMultiplier * DiageticVolumeMultiplier * VolumeMultiplier);
}

void USoundManager::PlayUISound(FName SoundName, UObject *WorldContextObject, bool bPersistOnLevelLoad)
{
    UAudioComponent* AudioComponent = CreateAudioComponent(SoundName, WorldContextObject);
    CHECK_NULLPTR_RET(AudioComponent, LogAudio, "USoundManager:: Failed to create AudioComponent!");

    AudioComponent->VolumeMultiplier = MasterVolumeMultiplier * UIVolumeMultiplier;
    AudioComponent->bAllowSpatialization = false;
    AudioComponent->bIsUISound = true;
    AudioComponent->bAutoDestroy = true;
    AudioComponent->bIgnoreForFlushing = bPersistOnLevelLoad;
    AudioComponent->bStopWhenOwnerDestroyed = !bPersistOnLevelLoad;
    AudioComponent->Play();
}

void USoundManager::PlayMusicOrAmbientSound(FName SoundName, UObject* WorldContextObject, bool bIsMusic, bool bPersistOnLevelLoad)
{
    TMap<FName, UAudioComponent*>* SoundMap = bIsMusic ? &ActiveMusic : &ActiveAmbience;
    CHECK_NULLPTR_RET(SoundMap, LogAudio, "SoundManager:: Failed to find relevant sound map!");

    UAudioComponent* AudioComponent;
    if (UAudioComponent** MapValue = (*SoundMap).Find(SoundName))
    {
        AudioComponent = *(MapValue);
        if (AudioComponent != nullptr) {AudioComponent->Play();}
        else
        {
            (*SoundMap).Remove(SoundName);
            PlayMusicOrAmbientSound(SoundName, WorldContextObject, bPersistOnLevelLoad);
        }
    }
    else
    {
        AudioComponent = CreateAudioComponent(SoundName, WorldContextObject);
        CHECK_NULLPTR_RET(AudioComponent, LogAudio, "USoundManager:: Failed to create AudioComponent!");

        AudioComponent->VolumeMultiplier = MasterVolumeMultiplier * (bIsMusic ? MusicVolumeMultiplier : AmbienceVolumeMultiplier);
        AudioComponent->bAllowSpatialization = false;
        AudioComponent->bIsUISound = true;
        AudioComponent->bIgnoreForFlushing = bPersistOnLevelLoad;
        AudioComponent->bStopWhenOwnerDestroyed = !bPersistOnLevelLoad;
        AudioComponent->bDisableParameterUpdatesWhilePlaying = false;
        AudioComponent->bIsMusic = true;
        AudioComponent->bCanPlayMultipleInstances = false;
        AudioComponent->Play();

        (*SoundMap).Add(SoundName, AudioComponent);
    }
}

UAudioComponent* USoundManager::CreateAudioComponent(FName SoundName, UObject* WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    CHECK_NULLPTR_RETVAL(World, LogEngine, "USoundManager:: Engine failed to get World from context Object!", nullptr);

    FAudioDevice* WorldAudioDevice = World->GetAudioDevice().GetAudioDevice();
    CHECK_NULLPTR_RETVAL(WorldAudioDevice, LogAudio, "USoundManager:: Could not get AudioDevice!", nullptr);

    USoundBase** Sound = SoundData.Find(SoundName);
    CHECK_NULLPTR_RETVAL(Sound, LogAudio, "USoundManager:: Invalid SoundName!", nullptr);
    CHECK_NULLPTR_RETVAL(*Sound, LogAudio, "USoundManager:: No Sound asset assigned to SoundName!", nullptr);

    return FAudioDevice::CreateComponent(*Sound, WorldAudioDevice);
}

void USoundManager::UpdateMusicVolume()
{
    TArray<UAudioComponent*> AudioComponents;
    ActiveMusic.GenerateValueArray(AudioComponents);
    for (UAudioComponent* AudioComponent: AudioComponents)
    {
        AudioComponent->SetVolumeMultiplier(MasterVolumeMultiplier * MusicVolumeMultiplier);
    }
}

void USoundManager::UpdateAmbientVolume()
{
    TArray<UAudioComponent*> AudioComponents;
    ActiveAmbience.GenerateValueArray(AudioComponents);
    for (UAudioComponent* AudioComponent: AudioComponents)
    {
        AudioComponent->SetVolumeMultiplier(MasterVolumeMultiplier * AmbienceVolumeMultiplier);
    }
}
