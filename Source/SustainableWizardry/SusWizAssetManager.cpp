// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAssetManager.h"
#include "SusWizGameplayTags.h"

USusWizAssetManager& USusWizAssetManager::Get()
{
	check(GEngine);
	
	USusWizAssetManager* SusWizAssetManager = Cast<USusWizAssetManager>(GEngine->AssetManager);
	return *SusWizAssetManager;
}

void USusWizAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FSusWizGameplayTags::InitializeNativeGameplayTags();
	
}
