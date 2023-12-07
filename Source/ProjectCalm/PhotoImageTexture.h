// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "PhotoImageTexture.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UPhotoImageTexture : public UTextureRenderTarget2D
{
	GENERATED_BODY()

	UPhotoImageTexture();

	// Implemented as workaround for Linker bug in UE 5.1.1 (identical to parent class function)
	virtual uint32 CalcTextureMemorySizeEnum(ETextureMipCount Enum) const override;

	bool bIsReady{false};
	
};
