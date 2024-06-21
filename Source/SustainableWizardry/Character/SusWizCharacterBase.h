// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"
#include "SustainableWizardry/GAS/Data/CharacterClassInfo.h"
#include "SusWizCharacterBase.generated.h"


// Forward declare for GAS
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
class UAnimMontage;

UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizCharacterBase : public ACharacter,
public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASusWizCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	

	

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	/** Combat Interface */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;	
	virtual void Die(const FVector& DeathImpulse) override;	
	virtual FVector GetCombatSocketLocation() override;
	virtual FVector GetSecCombatSocketLocation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;


	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual USkeletalMeshComponent* GetMainWeapon_Implementation() override;
	virtual USkeletalMeshComponent* GetSecWeapon_Implementation() override;
	/** end Combat Interface */

	bool bDead = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
	// Create the weapon mesh component to allow us to spawn spells from hand
	UPROPERTY(EditAnywhere, Category = "Combat")
	USkeletalMeshComponent* MainWeapon;
	UPROPERTY(EditAnywhere, Category = "Combat")
	USkeletalMeshComponent* SecondaryWeapon;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName MainWeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName SecondaryWeaponTipSocketName;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


	// 6.0 virtual InitAbilities for ASC. Override in player character
	virtual void InitAbilityActorInfo();

	// // 11.0 Creating default attributes on the character...
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	// // 12.0 Add secondary Attributes too! Make sure these go after Primary.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	// 15 Add vital attributes.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// 16 Implement above code
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	
	void AddCharacterAbilities();

	/* Dissolve Effects */

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	private:

	UPROPERTY(EditAnywhere, Category = "Abilities|Startup")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities|Startup")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
};
