// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 * GameplayTags
 * Singleton Native
 */
struct FSusWizGameplayTags
{
public:
static const FSusWizGameplayTags& Get() { return GameplayTags; }

 static void InitializeNativeGameplayTags();

 // Primary Attribute Tags

 FGameplayTag Attributes_Primary_Flare;
 FGameplayTag Attributes_Primary_Swift;
 FGameplayTag Attributes_Primary_Seismic;
 FGameplayTag Attributes_Primary_Deep;

 // Secondary Attribute Tags
 // TODO: Alex/Jeff add tags for attributes you have been working with.
 FGameplayTag Attributes_Secondary_Armor;
 FGameplayTag Attributes_Secondary_ArmorPen;
 FGameplayTag Attributes_Secondary_MaxEnergy;
 FGameplayTag Attributes_Secondary_MaxHealth;

 // Vital Attribute Tags
 FGameplayTag Attributes_Vital_Health;
 FGameplayTag Attributes_Vital_Energy;
 
 // Input Action Tags
 FGameplayTag InputTag_LMB;
 FGameplayTag InputTag_RMB;
 FGameplayTag InputTag_1;
 FGameplayTag InputTag_2;
 FGameplayTag InputTag_3;
 FGameplayTag InputTag_4;

 
 
protected:

private:
 static FSusWizGameplayTags GameplayTags;
};