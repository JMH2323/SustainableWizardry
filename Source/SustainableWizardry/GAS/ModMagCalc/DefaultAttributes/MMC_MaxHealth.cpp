// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxHealth.h"

#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{

	// Tell our program what it's capturing.
	DeepDef.AttributeToCapture = USusWizAttributeSet::GetDeepAttribute();
	// Define if we are applying the effect to the target or the source
	DeepDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// Define snapshot to define when attribute is applied on the effect.
	// For instance, when we cast a spell, do we want to use the damage attribute before the spell hits or after?
	DeepDef.bSnapshot = false;

	// Once defined, we add to capture.
	RelevantAttributesToCapture.Add(DeepDef);
	
}


float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{

	// Alternative method for calculating effects through TAGS.
	// Gather tags from source/target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	// Define what our magnitude is or where we get it
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Gains value of Deep attribute on target!
	float Deep = 0.f;
	GetCapturedAttributeMagnitude(DeepDef, Spec, EvaluationParameters, Deep);
	// Clamps the value if needed.
	Deep = FMath::Max<float>(Deep, 0.f);

	// Now that we've got Deep, Let's get the player level to add to the health modifier.
	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}


	/* MATH */ 
	// Use our deep attribute to do math.
	// In this case, base max of 80 + Deep adds 10 + Player level adds 10
	return 80.f + (10.f * Deep) + (10.f * PlayerLevel);
	
}
