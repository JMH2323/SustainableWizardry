// Fill out your copyright notice in the Description page of Project Settings.


#include "PurityBall.h"

FString UPurityBall::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			// Title
			"<Title>PURITY</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Energy Cost
			"<Small>Energy Cost: </><Energy>HALF your total energy</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>After a long channelling, "
			"fire a large and slow moving orb of pure energy. All enemies take </>"

			// Damage
			"<Damage>%d</><Default> Pure damage when passed through.</>"
			
			),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			ScaledDamage);
}

FString UPurityBall::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			// Title
			"<Title>PURITY</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Energy Cost
			"<Small>Energy Cost: </><Energy>HALF your total energy</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>After a long channelling, "
			"fire a large and slow moving orb of pure energy. All enemies take </>"

			// Damage
			"<Damage>%d</><Default> Pure damage when passed through.</>"
			
			),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			ScaledDamage);
}
