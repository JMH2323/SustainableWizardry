// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizGameplayTags.h"
#include "GameplayTagsManager.h"

FSusWizGameplayTags FSusWizGameplayTags::GameplayTags;

void FSusWizGameplayTags::InitializeNativeGameplayTags()
{
	// kickoff initializing gameplay tags by getting the gameplay tag manager and adding tags to it.
		
	// Primary Attributes
	GameplayTags.Attributes_Primary_Flare = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Flare"), FString ("Increases Damage/MaxEnergy"));
	GameplayTags.Attributes_Primary_Swift = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Swift"), FString ("Increases Speed/Dodge"));
	GameplayTags.Attributes_Primary_Seismic = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Seismic"), FString ("Increases Armor/ArmorPen"));
	GameplayTags.Attributes_Primary_Deep = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Deep"), FString ("Increases MaxHealth/Healing"));


	// Secondary Attributes
	// TODO: Alex/Jeff add your attributes. NOTE: Tags wont view with Live coding since this is created in engine launch, so you have to stop and start
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString ("Reduces Damage Taken"));
	GameplayTags.Attributes_Secondary_ArmorPen = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPen"), FString ("Reduces Armor Effect"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString ("Maximum Health Capacity"));
	GameplayTags.Attributes_Secondary_MaxEnergy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxEnergy"), FString ("Maximum Energy Capacity"));
	GameplayTags.Attributes_Secondary_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Dodge"), FString (" na "));
	GameplayTags.Attributes_Secondary_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalChance"), FString ("na"));
	GameplayTags.Attributes_Secondary_DamageScaling = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.DamageScaling"), FString ("na"));
	GameplayTags.Attributes_Secondary_HealingScaling = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealingScaling"), FString ("na"));
	
	
	// Vital Attributes
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"), FString ("Health"));
	GameplayTags.Attributes_Vital_Energy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Energy"), FString ("Energy"));
	
	
	// Input Config Tags
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString ("LMB"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString ("RMB"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString ("1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString ("2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString ("3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString ("4"));

	// Ability Tags
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString ("Attack Ability Tag"));

	// Ability Info
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString ("Locked Status"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString ("Eligible Status"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString ("Unlocked Status"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), FString ("Equipped Status"));

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Info.Passive"), FString ("Passive Type"));
	GameplayTags.Abilities_Type_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Info.Active"), FString ("Active Type"));
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Info.None"), FString ("No Type"));
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"), FString ("No Ability"));

	
	// Meta Tags
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString ("Damage"));

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"),FString("Fire Damage Type"));
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire);

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"),FString("Physical Damage Type"));

	// Resistance

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),FString("Resistance to Fire damage"));

	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),FString("Resistance to Physical damage"));

	/*
	 * Map of Damage Types to Resistances
	 */
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	
	// Effect Tags
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Tag granted when Hit Reacting"));
	
	// Meta Tags
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"),FString("Incoming XP Meta Attribute"));

	/*
	 * Spell Ability Tags
	 */
	// Fire
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString ("FireBolt Ability Tag"));
	GameplayTags.Abilities_Fire_BigFireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.BigFireBolt"), FString ("FireBolt Ability Tag"));
	// Solar
	GameplayTags.Abilities_Solar_Flare = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Solar.Flare"), FString ("SOLAR Ability Tag"));
	GameplayTags.Abilities_Solar_Beam = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Solar.Beam"), FString ("SOLAR Ability Tag"));
	GameplayTags.Abilities_Solar_EverPresence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Solar.EverPresence"), FString ("SOLAR Ability Tag"));
	// Geo
	GameplayTags.Abilities_Geo_RockPunch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Geo.RockPunch"), FString ("GEO Ability Tag"));
	GameplayTags.Abilities_Geo_ForceOfEarth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Geo.ForceOfEarth"), FString ("GEO Ability Tag"));
	GameplayTags.Abilities_Geo_Boulder = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Geo.Boulder"), FString ("GEO Ability Tag"));
	// Hydro
	GameplayTags.Abilities_Hydro_HydroPulse = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Hydro.HydroPulse"), FString ("HYDRO Ability Tag"));
	GameplayTags.Abilities_Hydro_WaterWhip = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Hydro.WaterWhip"), FString ("HYDRO Ability Tag"));
	GameplayTags.Abilities_Hydro_OceanCall = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Hydro.OceanCall"), FString ("HYDRO Ability Tag"));

	// Wind
	GameplayTags.Abilities_Wind_Cut = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Wind.Cut"), FString ("WIND Ability Tag"));
	GameplayTags.Abilities_Wind_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Wind.Dash"), FString ("WIND Ability Tag"));
	GameplayTags.Abilities_Wind_InstantCarnage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Wind.InstantCarnage"), FString ("WIND Ability Tag"));

	/*
	 * Spell Cooldown Tags
	 */
	// Fire
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString ("FireBolt Cooldown Tag"));
	GameplayTags.Cooldown_Fire_BigFireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.BigFireBolt"), FString ("FireBolt Cooldown Tag"));
	// Solar
	GameplayTags.Cooldown_Solar_Flare = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Solar.Flare"), FString ("SOLAR Cooldown Tag"));
	GameplayTags.Cooldown_Solar_Beam = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Solar.Beam"), FString ("SOLAR Cooldown Tag"));
	GameplayTags.Cooldown_Solar_EverPresence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Solar.EverPresence"), FString ("SOLAR Cooldown Tag"));
	// Geo
	GameplayTags.Cooldown_Geo_RockPunch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Geo.RockPunch"), FString ("GEO Cooldown Tag"));
	GameplayTags.Cooldown_Geo_ForceOfEarth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Geo.ForceOfEarth"), FString ("GEO Cooldown Tag"));
	GameplayTags.Cooldown_Geo_Boulder = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Geo.Boulder"), FString ("GEO Cooldown Tag"));
	// Hydro
	GameplayTags.Cooldown_Hydro_HydroPulse = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Hydro.HydroPulse"), FString ("HYDRO Cooldown Tag"));
	GameplayTags.Cooldown_Hydro_WaterWhip = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Hydro.WaterWhip"), FString ("HYDRO Cooldown Tag"));
	GameplayTags.Cooldown_Hydro_OceanCall = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Hydro.OceanCall"), FString ("HYDRO Cooldown Tag"));

	// Wind
	GameplayTags.Cooldown_Wind_Cut = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Wind.Cut"), FString ("WIND Cooldown Tag"));
	GameplayTags.Cooldown_Wind_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Wind.Dash"), FString ("WIND Cooldown Tag"));
	GameplayTags.Cooldown_Wind_InstantCarnage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Wind.InstantCarnage"), FString ("WIND Cooldown Tag"));


	
}