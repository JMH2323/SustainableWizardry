// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SusWizAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizAssetManager : public UAssetManager
{
	GENERATED_BODY()


public:

	static USusWizAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
	
};
