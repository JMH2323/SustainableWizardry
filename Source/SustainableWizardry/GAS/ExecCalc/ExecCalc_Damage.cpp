// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"

#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "AbilitySystemComponent.h"

struct SusWizDamageStatics
{

	// Creates the capture def to be defined below
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	
	SusWizDamageStatics()
	{
		// Defined the armor capture definition. Set, def, source, and boolean(snapshot)
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, Armor, Target, false);
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

	// Local Attributes initialized for population and use
	float Armor = 0.f;
	
	// Get those attributes from the capture list and clamp them if needed
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, Armor);
	Armor = FMath::Max<float>(0.f, Armor);


	// DO MATH


	// Output as Evaluated Data
	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, Armor);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
	
}
