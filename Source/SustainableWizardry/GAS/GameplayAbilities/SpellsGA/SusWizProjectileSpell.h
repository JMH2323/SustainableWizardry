// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SusWizGameplayAbility.h"
#include "SustainableWizardry/GAS/GameplayAbilities/DamageAbility/SusWizDamageGameplayAbility.h"
#include "SusWizProjectileSpell.generated.h"

/**
 * 
 */
class ASusWizProjectiles;
class UGameplayEffect;
UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizProjectileSpell : public USusWizDamageGameplayAbility
{
	GENERATED_BODY()

	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile();
	const FGameplayAbilityActorInfo *StoredActorInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASusWizProjectiles> ProjectileClass;
	
	
	
};
