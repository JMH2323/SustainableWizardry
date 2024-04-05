// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"

#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "VectorVM.h"
#include "SustainableWizardry/SusWizGameplayTags.h"

struct SusWizDamageStatics
{

	// Creates the capture def to be defined below
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPen);
	
	SusWizDamageStatics()
	{
		// Defined the armor capture definition. Set, def, source, and boolean(snapshot)
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, ArmorPen, Source, false);
	}
};

static const SusWizDamageStatics& DamageStatics()
{
	static SusWizDamageStatics DStatics;
	return DStatics;
	
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// Add the definitions to the attribute capture. lets us finds those attributes we want from the definitions.
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	// Get the source and target's ASC
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// Get the Avatars from the ASCs if they are valid
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// Get the owning gameplay effect specs
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Use specs to get captured and associated gameplay tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Set our evaluation parameters to use those tags
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;


	// Get Damage set by the caller magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FSusWizGameplayTags::Get().Damage);
	

	


	/*
	 * DO MATH
	 * Scale first
	 * Armor second
	 * Dodge last
	 */

	/* TODO: Scale Damage?? */

	
	/* Armor */
	// Capture armor and armor penetration. Reduce damage by percentage
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPen = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenDef, EvaluateParameters, SourceArmorPen);
	SourceArmorPen = FMath::Max<float>(SourceArmorPen, 0.f);

	float ArmorApplied = TargetArmor - SourceArmorPen;
	
	Damage *= (1.0f - (ArmorApplied / 100.f));

	
	/* TODO: Dodge Chance */
	// Capture dodge change, if dodged, negate all damage.
	float TargetDodgeChance = 0.f;
	
	// Use this bool for dodging
	const bool bDodged = FMath::RandRange(1, 100) < TargetDodgeChance;
	Damage = bDodged ? Damage = 0.f : Damage;


	/*
	 * Math done
	 */
	
	// Output as Evaluated Data
	const FGameplayModifierEvaluatedData EvaluatedData(USusWizAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
	
}
