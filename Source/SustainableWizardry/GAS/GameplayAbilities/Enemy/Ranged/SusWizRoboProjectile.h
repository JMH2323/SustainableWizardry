#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SusWizGameplayAbility.h"
#include "SustainableWizardry/GAS/GameplayAbilities/DamageAbility/SusWizDamageGameplayAbility.h"
#include "SusWizRoboProjectile.generated.h"

class ASusWizProjectiles;
class UGameplayEffect;

UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizRoboProjectile : public USusWizDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASusWizProjectiles> ProjectileClass;
	
};
