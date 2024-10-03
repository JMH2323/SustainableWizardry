// Fill out your copyright notice in the Description page of Project Settings.


#include "SolarBeam.h"

FString USolarBeam::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 BeamNumber = GetNumberOfBeamsAtLevel(Level);
	
		return FString::Printf(TEXT(
			// Title
			"<Title>SOLAR BEAM</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Energy Cost
			"<Small>Energy Cost: </><Energy>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Hold to channel a targeting circle. "
			// Beam Count
			"Upon release, call down </><Energy>%i</>, beams from the sun. Each deal"

			// Damage
			"<Damage>%d</><Default> Solar damage at their center, and singe those close to them.</>"),
			
			// Values
			Level,
			EnergyCost,
			Cooldown,
			BeamNumber,
			ScaledDamage);
}

FString USolarBeam::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 BeamNumber = GetNumberOfBeamsAtLevel(Level);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>SOLAR BEAM</>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// Energy Cost
		"<Small>Energy Cost: </><Energy>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		"<Default>Hold to channel a targeting circle. "
		// Beam Count
		"Upon release, call down </><Energy>%i</>, beams from the sun. Each deal"

		// Damage
		"<Damage>%d</><Default> Solar damage at their center, and singe those close to them.</>"),
			
		// Values
		Level,
		EnergyCost,
		Cooldown,
		BeamNumber,
		ScaledDamage);
}

void USolarBeam::SetNumberOfBeamsToSpawn(int32 Level)
{
	switch (Level)
	{
	case 0:
		NumberOfBeams = 1;
		break;
	case 1:
		NumberOfBeams = 1;
		break;
	case 2:
		NumberOfBeams = 3;
		break;
	case 3:
		NumberOfBeams = 5;
		break;
	case 4:
		NumberOfBeams = 7;
		break;
	default:
		NumberOfBeams = 10;
		break;
	}
}

int32 USolarBeam::GetNumberOfBeamsAtLevel(int32 Level)
{
	switch (Level)
	{
	case 0:
		return 1;
		break;
	case 1:
		return 1;
		break;
	case 2:
		return 3;
		break;
	case 3:
		return 5;
		break;
	case 4:
		return 7;
		break;
	default:
		return 10;
		break;
	}
}
