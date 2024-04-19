// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"

#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "VectorVM.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/GAS/Data/CharacterClassInfo.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SusWizAbilityTypes.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"

struct SusWizDamageStatics
{

	// Creates the capture def to be defined below
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPen);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dodge);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageScale);
	
	SusWizDamageStatics()
	{
		// Defined capture definition. Set, def, source, and boolean(snapshot)
		/* Armor */
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, ArmorPen, Source, false);
		/* TODONE: Crit */
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, CriticalChance, Source, false);
		/* TODONE: Dodge */
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, Dodge, Target, false);
		/* TODONE: DamageScale */
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, DamageScale, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().DodgeDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageScaleDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	// Get the source and target's ASC
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// Get the Avatars from the ASCs if they are valid
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	// Get Combat Interface for level interaction
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	
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

	// Character class information in case we want to scale values in UE (levels, etc.)
	const UCharacterClassInfo* SourceCharacterClassInfo = USusWizAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	// Example:
	//const FRealCurve* /*CurveNameCurve*/ CharacterClassInfo->DamageCalculationCoefficients->FindCurve(/*FName("CurveHere"), FString()/*)
	//const float /*CurveNameCoefficient*/ = /*CurveNameCurve*/->Eval(/*Source/Target*/CombatInterface->GetPlayerLevel());
	// Use Coefficient in math^^ TODO: MAKE SURE YOU MAKE THE CURVE TABLE AND ASSIGN IT IF USED


	/* TODONE: Scale Damage */
	float SourceDamageScale = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, SourceDamageScale);
	SourceDamageScale = FMath::Max<float>(SourceDamageScale, 0.f);

	Damage *= (1.f + (SourceDamageScale * 0.01));
	
	/* Armor */
	// Capture armor and armor penetration. Reduce damage by percentage
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPen = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenDef, EvaluateParameters, SourceArmorPen);
	SourceArmorPen = FMath::Max<float>(SourceArmorPen, 0.f);

	float ArmorApplied = TargetArmor - SourceArmorPen;
	ArmorApplied = (ArmorApplied <= 0.f) ? ArmorApplied = 0.f : ArmorApplied;
	Damage *= (1.0f - (ArmorApplied / 100.f)); 

	/* TODONE: Crit Chance */
	// Capture Crit change, if crit, double all damage.
	
	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalChanceDef, EvaluateParameters, SourceCriticalChance);
	
	const bool bCrit = FMath::RandRange(1,100) < SourceCriticalChance;
	Damage = bCrit ? Damage *= 2.f : Damage;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	USusWizAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCrit);
	
	/* TODONE: Dodge Chance */
	// Capture dodge change, if dodged, negate all damage.
	float TargetDodgeChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DodgeDef, EvaluateParameters, TargetDodgeChance);
	// Use this bool for dodging
	const bool bDodged = FMath::RandRange(1, 100) < TargetDodgeChance;
	// Set damage. if dodged is true = 0. otherwise, keep it as it is.
	Damage = bDodged ? Damage = 1.f : Damage;
	
	USusWizAbilitySystemLibrary::SetIsDodgedHit(EffectContextHandle, bDodged);

	/*
	 * Math done
	 */
	
	// Output as Evaluated Data
	const FGameplayModifierEvaluatedData EvaluatedData(USusWizAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
	
}
