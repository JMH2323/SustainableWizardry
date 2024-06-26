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
	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Q"), FString ("Q"));
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.E"), FString ("E"));

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

	
	// Damage Tags
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString ("Damage"));
	GameplayTags.Damage_Pure = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Pure"),FString("Pure Damage Type"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"),FString("Physical Damage Type"));
	GameplayTags.Damage_Hydro = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Hydro"),FString("Hydro Damage Type"));
	GameplayTags.Damage_Solar = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Solar"),FString("Solar Damage Type"));
	GameplayTags.Damage_Rock = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Rock"),FString("Rock Damage Type"));
	GameplayTags.Damage_Aero = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Aero"),FString("Aero Damage Type"));

	// Resistance
	GameplayTags.Attributes_Resistance_Pure = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Pure"),FString("Resistance to Pure damage"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),FString("Resistance to Physical damage"));
	GameplayTags.Attributes_Resistance_Hydro = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Hydro"),FString("Resistance to Hydro damage"));
	GameplayTags.Attributes_Resistance_Solar = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Solar"),FString("Resistance to Solar damage"));
	GameplayTags.Attributes_Resistance_Rock = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Rock"),FString("Resistance to Rock damage"));
	GameplayTags.Attributes_Resistance_Aero = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Aero"),FString("Resistance to Aero damage"));

	// Debuffs
	GameplayTags.Debuff_Bleed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Bleed"),FString("Bleed Debuff"));
	GameplayTags.Debuff_Lit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Lit"),FString("Lit Debuff"));
	GameplayTags.Debuff_Wet = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Wet"),FString("Wet Debuff"));
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"),FString("Stun Debuff"));
	// Debuff Params
	GameplayTags.Debuff_Param_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Param.Chance"),FString("Debuff Chance"));
	GameplayTags.Debuff_Param_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Param.Damage"),FString("Debuff Damage"));
	GameplayTags.Debuff_Param_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Param.Duration"),FString("Debuff Duration"));
	GameplayTags.Debuff_Param_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Param.Frequency"),FString("Debuff Frequency"));
	
	/*
	 * Map of Damage Types to Resistances
	 */
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Pure, GameplayTags.Attributes_Resistance_Pure);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Solar, GameplayTags.Attributes_Resistance_Solar);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Rock, GameplayTags.Attributes_Resistance_Rock);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Aero, GameplayTags.Attributes_Resistance_Aero);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Hydro, GameplayTags.Attributes_Resistance_Hydro);
	
	/*
	 * Map of Damage Types to Debuffs
	 */
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Hydro, GameplayTags.Debuff_Wet);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Rock, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Aero, GameplayTags.Debuff_Bleed);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Solar, GameplayTags.Debuff_Lit);
	
	// Effect Tags
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Tag granted when Hit Reacting"));
	
	// Meta Tags
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"),FString("Incoming XP Meta Attribute"));

	/*
	* Player Tags
	*/
	GameplayTags.Player_Block_LTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.LTrace"),FString("Block tracing"));
	GameplayTags.Player_Block_LInputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.LInputHeld"),FString("Block Left Input"));
	GameplayTags.Player_Block_LInputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.LInputPressed"),FString("Block Left Input"));
	GameplayTags.Player_Block_LInputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.LInputReleased"),FString("Block Left Input"));
	GameplayTags.Player_Block_RTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.RTrace"),FString("Block tracing"));
	GameplayTags.Player_Block_RInputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.RInputHeld"),FString("Block Right Input"));
	GameplayTags.Player_Block_RInputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.RInputPressed"),FString("Block Right Input"));
	GameplayTags.Player_Block_RInputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.RInputReleased"),FString("Block Right Input"));
	
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

	// Aero
	GameplayTags.Abilities_Aero_Cut = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Aero.Airrow"), FString ("Aero Ability Tag"));
	GameplayTags.Abilities_Aero_Cut = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Aero.Cut"), FString ("Aero Ability Tag"));
	GameplayTags.Abilities_Aero_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Aero.Dash"), FString ("Aero Ability Tag"));
	GameplayTags.Abilities_Aero_InstantCarnage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Aero.InstantCarnage"), FString ("Aero Ability Tag"));

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

	// Aero
	GameplayTags.Cooldown_Aero_Cut = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Aero.Airrow"), FString ("Aero Cooldown Tag"));
	GameplayTags.Cooldown_Aero_Cut = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Aero.Cut"), FString ("Aero Cooldown Tag"));
	GameplayTags.Cooldown_Aero_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Aero.Dash"), FString ("Aero Cooldown Tag"));
	GameplayTags.Cooldown_Aero_InstantCarnage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Aero.InstantCarnage"), FString ("Aero Cooldown Tag"));

	/* Spells Extra */
	GameplayTags.Abilities_Hydro_HydroPulse_BlockLEffect = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Hydro.HydroPulse.BlockLEffect"),
		FString ("The input blocking effect for the spell casting left handed"));
	GameplayTags.Abilities_Hydro_HydroPulse_BlockREffect = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Hydro.HydroPulse.BlockREffect"),
		FString ("The input blocking effect for the spell casting right handed"));
	GameplayTags.Abilities_Aero_Airrow_BlockLEffect = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Aero.Airrow.BlockLEffect"),
		FString ("The input blocking effect for the spell casting left handed"));
	GameplayTags.Abilities_Aero_Airrow_BlockREffect = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Aero.Airrow.BlockREffect"),
		FString ("The input blocking effect for the spell casting right handed"));
	
}