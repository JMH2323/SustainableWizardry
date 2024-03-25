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

	// Get all the data effect properties
	void SetEffectProperties(const FGameplayEffectModCallbackData Data, FEffectProperties& Props) const;

	// PART ONE VERSION
	void EmptyPartOne(const FGameplayEffectModCallbackData& Data);

	
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
	// TODO: Alex = Healing scale, Damage scale (Deep/Swift)


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

	
};
