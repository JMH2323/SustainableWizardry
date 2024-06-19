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
 FGameplayTag InputTag_Q;
 FGameplayTag InputTag_E;

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
 FGameplayTag Abilities_None;

 /*
  * Damage Types
  */
 FGameplayTag Damage;
 FGameplayTag Damage_Hydro;
 FGameplayTag Damage_Solar;
 FGameplayTag Damage_Pure;
 FGameplayTag Damage_Physical;
 FGameplayTag Damage_Rock;
 FGameplayTag Damage_Aero;

 // Resistances
 FGameplayTag Attributes_Resistance_Physical;
 FGameplayTag Attributes_Resistance_Pure;
 FGameplayTag Attributes_Resistance_Solar;
 FGameplayTag Attributes_Resistance_Hydro;
 FGameplayTag Attributes_Resistance_Rock;
 FGameplayTag Attributes_Resistance_Aero;

 // Debuffs
 FGameplayTag Debuff_Bleed;
 FGameplayTag Debuff_Stun;
 FGameplayTag Debuff_Lit;
 FGameplayTag Debuff_Wet;

 // Debuff Params
 FGameplayTag Debuff_Param_Chance;
 FGameplayTag Debuff_Param_Damage;
 FGameplayTag Debuff_Param_Duration;
 FGameplayTag Debuff_Param_Frequency;

 // Damage Mapping
 TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
 TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
 
 
 // Meta
 FGameplayTag Attributes_Meta_IncomingXP;
 
 // Effects
 FGameplayTag Effects_HitReact;

/*
 * Player Tags
 */

 // Blocking Tags for preventing multiple spell overlaps for Left hand
 FGameplayTag Player_Block_LInputPressed;
 FGameplayTag Player_Block_LInputHeld;
 FGameplayTag Player_Block_LInputReleased;
 FGameplayTag Player_Block_LTrace;
 // Right hand casting
 FGameplayTag Player_Block_RInputPressed;
 FGameplayTag Player_Block_RInputHeld;
 FGameplayTag Player_Block_RInputReleased;
 FGameplayTag Player_Block_RTrace;

 /* 
  * Spell Abilities
  */
 // Starting Fire 
 FGameplayTag Abilities_Fire_FireBolt;
 FGameplayTag Abilities_Fire_BigFireBolt;
 // Hydro
 FGameplayTag Abilities_Hydro_HydroPulse;
 FGameplayTag Abilities_Hydro_WaterWhip;
 FGameplayTag Abilities_Hydro_OceanCall;
 // Geo
 FGameplayTag Abilities_Geo_RockPunch;
 FGameplayTag Abilities_Geo_ForceOfEarth;
 FGameplayTag Abilities_Geo_Boulder;
 // Solar
 FGameplayTag Abilities_Solar_Flare;
 FGameplayTag Abilities_Solar_Beam;
 FGameplayTag Abilities_Solar_EverPresence;
 // Wind
 FGameplayTag Abilities_Wind_Cut;
 FGameplayTag Abilities_Wind_Dash;
 FGameplayTag Abilities_Wind_InstantCarnage;

 /* 
  * Spell Cooldowns
  */
 // Starting Fire 
 FGameplayTag Cooldown_Fire_FireBolt;
 FGameplayTag Cooldown_Fire_BigFireBolt;
 // Hydro
 FGameplayTag Cooldown_Hydro_HydroPulse;
 FGameplayTag Cooldown_Hydro_WaterWhip;
 FGameplayTag Cooldown_Hydro_OceanCall;
 // Geo
 FGameplayTag Cooldown_Geo_RockPunch;
 FGameplayTag Cooldown_Geo_ForceOfEarth;
 FGameplayTag Cooldown_Geo_Boulder;
 // Solar
 FGameplayTag Cooldown_Solar_Flare;
 FGameplayTag Cooldown_Solar_Beam;
 FGameplayTag Cooldown_Solar_EverPresence;
 // Wind
 FGameplayTag Cooldown_Wind_Cut;
 FGameplayTag Cooldown_Wind_Dash;
 FGameplayTag Cooldown_Wind_InstantCarnage;


 /*
  * Spells Extras
  */
 FGameplayTag Abilities_Hydro_HydroPulse_BlockLEffect;
 FGameplayTag Abilities_Hydro_HydroPulse_BlockREffect;


 
protected:

private:
 static FSusWizGameplayTags GameplayTags;
};