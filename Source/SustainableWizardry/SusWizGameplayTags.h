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
 FGameplayTag Attributes_Secondary_Dodge; 
 FGameplayTag Attributes_Secondary_CriticalChance;
 FGameplayTag Attributes_Secondary_DamageScaling; 
 FGameplayTag Attributes_Secondary_HealingScaling;

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

 // Abilities
 FGameplayTag Abilities_Attack;

 // Ability Info
 FGameplayTag Abilities_Status_Locked;
 FGameplayTag Abilities_Status_Eligible;
 FGameplayTag Abilities_Status_Unlocked;
 FGameplayTag Abilities_Status_Equipped;

 FGameplayTag Abilities_Type_Passive;
 FGameplayTag Abilities_Type_Active;
 FGameplayTag Abilities_Type_None;

 // Damage Types
 FGameplayTag Damage;
 FGameplayTag Damage_Fire;
 FGameplayTag Damage_Physical;
 // Array of Damages
 TArray<FGameplayTag> DamageTypes;
 TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

 // Res
 FGameplayTag Attributes_Resistance_Physical;
 FGameplayTag Attributes_Resistance_Fire;

 // Meta
 FGameplayTag Attributes_Meta_IncomingXP;
 
 //Misc
 FGameplayTag Effects_HitReact;
 

 // Abilities
 FGameplayTag Abilities_Fire_FireBolt;
 
protected:

private:
 static FSusWizGameplayTags GameplayTags;
};