// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "SusWizAttributeSet.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// VERY LATE STEP. Post Gameplay Accessors

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

// typedef is specific to the FGameplayAttribute() signature, but TStaticFunPtr is generic to any signature chosen
// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFunPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;


// TODO: READ THROUGH ATTRIBUTE SET COMMENTS!!!!
UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	USusWizAttributeSet();
	
	// Ensure replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Pre Attribute Change
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Post Gameplay Effect
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


	// Getting the attribute data from the tag using templatized FuncPtr from Unreal and handling GameplayAttributes
	TMap<FGameplayTag, TStaticFunPtr<FGameplayAttribute()>> TagsToAttributes;
	


private:

	void HandleIncomingDamage(const FEffectProperties& Props);
	void HandleIncomingXP(const FEffectProperties& Props);
	void Debuff(const FEffectProperties& Props);
	
	// Get all the data effect properties
	void SetEffectProperties(const FGameplayEffectModCallbackData Data, FEffectProperties& Props) const;

	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bDodgedHit, bool bCriticalHit);

	void SendXPEvent(const FEffectProperties& Props);

	
public:



	
	

	// Attribute Data
	// Steps) 1. Create attribute in .h file. 2. creating the OnRep_ Function. 3. Add to props list
	
	/*
	 * Primary Attributes
	 * RPG-Style Attributes for players.
	 */

	// Flare
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Flare, Category = "Primary Attributes")
	FGameplayAttributeData Flare;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, Flare);
	UFUNCTION()
	void OnRep_Flare(const FGameplayAttributeData& OldFlare) const;

	// Deep
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Deep, Category = "Primary Attributes")
	FGameplayAttributeData Deep;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, Deep);
	UFUNCTION()
	void OnRep_Deep(const FGameplayAttributeData& OldDeep) const;
	
	// Seismic
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Seismic, Category = "Primary Attributes")
	FGameplayAttributeData Seismic;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, Seismic);
	UFUNCTION()
	void OnRep_Seismic(const FGameplayAttributeData& OldSeismic) const;
	
	// Swift
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Swift, Category = "Primary Attributes")
	FGameplayAttributeData Swift;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, Swift);
	UFUNCTION()
	void OnRep_Swift(const FGameplayAttributeData& OldSwift) const;
	
	/*
	 * Secondary Attributes
	 */

	// Armor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, Armor);
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	// ArmorPen
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPen, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPen;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, ArmorPen);
	UFUNCTION()
	void OnRep_ArmorPen(const FGameplayAttributeData& OldArmorPen) const;

	// TODO: Jeff = Speed, dodge (Wind power/Swift)
	//CriticalChance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, CriticalChance);
	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const;

	//Dodge
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dodge, Category = "Secondary Attributes")
	FGameplayAttributeData Dodge;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, Dodge);
	UFUNCTION()
	void OnRep_Dodge(const FGameplayAttributeData& OldDodge) const;
	
	// TODO: Alex = Healing scale, Damage scale (Deep/Flare)

	//DamageScale
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageScale, Category = "Secondary Attributes")
	FGameplayAttributeData DamageScale;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, DamageScale);
	UFUNCTION()
	void OnRep_DamageScale(const FGameplayAttributeData& OldDamageScale) const;

	//CriticalChance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealingScale, Category = "Secondary Attributes")
	FGameplayAttributeData HealingScale;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, HealingScale);
	UFUNCTION()
	void OnRep_HealingScale(const FGameplayAttributeData& OldHealingScale) const;

	/*
	 * Vital Attributes
	 * Attributes required for everything.
	 */
	
	// Health 
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, Health);
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	// Max Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, MaxHealth);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	// Optional Energy for Stretch Goals
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Energy, Category = "Vital Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, Energy);
	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData& OldEnergy) const;

	// Max Energy
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy, Category = "Vital Attributes")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, MaxEnergy);
	UFUNCTION()
	void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const;


	/*
	 * Meta Attributes
	 * Attributes for the server
	 */

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, IncomingXP);


	/*
	 * Res
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PureResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PureResistance;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, PureResistance);
	UFUNCTION()
	void OnRep_PureResistance(const FGameplayAttributeData& OldPureResistance) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, PhysicalResistance);
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HydroResistance, Category = "Resistance Attributes")
	FGameplayAttributeData HydroResistance;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, HydroResistance);
	UFUNCTION()
	void OnRep_HydroResistance(const FGameplayAttributeData& OldHydroResistance) const;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RockResistance, Category = "Resistance Attributes")
	FGameplayAttributeData RockResistance;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, RockResistance);
	UFUNCTION()
	void OnRep_RockResistance(const FGameplayAttributeData& OldRockResistance) const;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AeroResistance, Category = "Resistance Attributes")
	FGameplayAttributeData AeroResistance;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, AeroResistance);
	UFUNCTION()
	void OnRep_AeroResistance(const FGameplayAttributeData& OldAeroResistance) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SolarResistance, Category = "Resistance Attributes")
	FGameplayAttributeData SolarResistance;
	ATTRIBUTE_ACCESSORS(USusWizAttributeSet, SolarResistance);
	UFUNCTION()
	void OnRep_SolarResistance(const FGameplayAttributeData& OldSolarResistance) const;

	
};
