// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyBolt.h"

#include "SustainableWizardry/SusWizGameplayTags.h"


FString UEnergyBolt::GetDescription(int32 Level)
{

	const int32 Damage = GetDamageByDamageType(Level, FSusWizGameplayTags::Get().Damage_Fire);
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ENERGY BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Energy Cost
			"<Small>Energy Cost: </><Energy>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Launches a bolt of  energy, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</><Default> pure damage</>"),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ENERGY BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Energy Cost: </><Energy>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Launches %d bolts of energy, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</><Default> pure damage</>"),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			Damage);

	}
	
}

FString UEnergyBolt::GetNextLevelDescription(int32 Level)
{
	// Get Damage
	const int32 Damage = GetDamageByDamageType(Level, FSusWizGameplayTags::Get().Damage_Fire);
	// Get Cost
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	// Get Cooldown
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			// Title
			"<Title>ENERGY BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Energy Cost: </><Energy>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Launches %d bolts of energy, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</><Default> pure damage</>"),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			Damage);

}
