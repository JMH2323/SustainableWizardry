// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyBolt.h"

#include "SustainableWizardry/SusWizGameplayTags.h"


FString UEnergyBolt::GetDescription(int32 Level)
{

	const int32 Damage = DamageTypes[FSusWizGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>ENERGY BOLT</>\n\n<Default>Launches a bolt of energy, exploding on impact "
		"and dealing: </><Damage>%d</><Default> pure damage</>\n\n<Small>Level: </><Level>%d</>"),
		Damage, Level);
	}
	else
	{
		return FString::Printf(TEXT("<Title>ENERGY BOLT</>\n\n<Default>Launches %d bolts of energy, exploding on impact "
		"and dealing: </><Damage>%d</><Default> pure damage</>\n\n<Small>Level: </><Level>%d</>"),
		FMath::Min(Level, NumProjectiles), Damage, Level);
	}
	
}

FString UEnergyBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = DamageTypes[FSusWizGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Title>ENERGY BOLT: </>\n\n<Default>Launches %d bolts of energy, exploding on impact "
	"and dealing: </><Damage>%d</><Default> pure damage</>\n\n<Small>Level: </><Level>%d</>"),
	FMath::Min(Level, NumProjectiles), Damage, Level);
}
