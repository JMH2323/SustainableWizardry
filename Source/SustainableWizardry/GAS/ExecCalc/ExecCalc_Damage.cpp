// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"

#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "VectorVM.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/GAS/Data/CharacterClassInfo.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SusWizAbilityTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeSourceActor.h"
#include "DSP/LFO.h"
#include "Kismet/KismetMathLibrary.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"

struct SusWizDamageStatics
{

	// Creates the capture def to be defined below
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPen);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dodge);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageScale);

	// Resistances
	DECLARE_ATTRIBUTE_CAPTUREDEF(PureResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HydroResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(SolarResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(RockResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AeroResistance);
	
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

		// Get Resistances from Target
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, PureResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, PhysicalResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, AeroResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, RockResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, HydroResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USusWizAttributeSet, SolarResistance, Target, false);

		const FSusWizGameplayTags& Tags = FSusWizGameplayTags::Get();
		
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

	RelevantAttributesToCapture.Add(DamageStatics().PureResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().HydroResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().SolarResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().AeroResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().RockResistanceDef);
}


///// MATH for Debuff
void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
						 const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();

	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (TypeDamage > -.5f) // .5 padding for floating point [im]precision
		{
			// Determine if there was a successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Param_Chance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * ( 100 - TargetDebuffResistance ) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				USusWizAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				USusWizAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Param_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Param_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Param_Frequency, false, -1.f);

				USusWizAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				USusWizAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				USusWizAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FSusWizGameplayTags& Tags = FSusWizGameplayTags::Get();
	
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Dodge, DamageStatics().DodgeDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPen, DamageStatics().ArmorPenDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalChance, DamageStatics().CriticalChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_DamageScaling, DamageStatics().DamageScaleDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Pure, DamageStatics().PureResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Hydro, DamageStatics().HydroResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Solar, DamageStatics().SolarResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Rock, DamageStatics().RockResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Aero, DamageStatics().AeroResistanceDef);

	// Get the source and target's ASC
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// Get the Avatars from the ASCs if they are valid
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}
	
	// Get the owning gameplay effect specs and context
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

	// Use specs to get captured and associated gameplay tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Set our evaluation parameters to use those tags
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Debuff
	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters, TagsToCaptureDefs);

	// Get Damage set by the caller magnitude
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair  : FSusWizGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		if(DamageTypeValue <= 0.f) {continue;}

		float Resistance = 1.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 1.f, 100.f);

		DamageTypeValue *= ( 100.f - Resistance ) / 100.f;

		// End Resistance
		// Begin Radial Damage
		if(USusWizAbilitySystemLibrary::IsRadialDamage(ContextHandle))
		{
			FVector TargetLocation = TargetAvatar->GetActorLocation();
			const FVector OriginLocation = USusWizAbilitySystemLibrary::GetRadialDamageOrigin(ContextHandle);
			TargetLocation.Z = OriginLocation.Z;
			const float SquareDistance = UKismetMathLibrary::Vector_DistanceSquared(TargetLocation, OriginLocation);
			const float SquareOuterRadius = FMath::Square(USusWizAbilitySystemLibrary::GetRadialDamageOuterRadius(ContextHandle));
			const float SquareInnerRadius = FMath::Square(USusWizAbilitySystemLibrary::GetRadialDamageInnerRadius(ContextHandle));

			if(SquareDistance > SquareOuterRadius)
			{
				Damage = 0.f;
				return;
			}
			if (SquareDistance <= SquareInnerRadius)
			{
				Damage += DamageTypeValue;
				continue;
			}
			else
			{
				DamageTypeValue = UKismetMathLibrary::MapRangeClamped(
			SquareDistance, SquareInnerRadius, SquareOuterRadius,
			DamageTypeValue, 1.f);	
			}
		}
		
		Damage += DamageTypeValue;

		
	}
	

	


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
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageScaleDef, EvaluateParameters, SourceDamageScale);
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
	// Set damage. if dodged is true = 1. otherwise, keep it as it is.
	Damage = bDodged ? Damage = 1.f : Damage;
	
	USusWizAbilitySystemLibrary::SetIsDodgedHit(EffectContextHandle, bDodged);

	/*
	 * Math done
	 */
	
	// Output as Evaluated Data
	const FGameplayModifierEvaluatedData EvaluatedData(USusWizAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
	
}
