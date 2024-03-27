// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SusWizGameplayAbility.h"
#include "SusWizProjectileSpell.generated.h"

/**
 * 
 */
class ASusWizProjectiles;
UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizProjectileSpell : public USusWizGameplayAbility
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
