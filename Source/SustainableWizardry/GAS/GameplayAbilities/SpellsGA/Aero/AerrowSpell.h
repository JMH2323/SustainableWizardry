// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SpellsGA/SusWizProjectileSpell.h"
#include "AerrowSpell.generated.h"

class AAerowProjectileClass;
/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API UAerrowSpell : public USusWizProjectileSpell
{
	GENERATED_BODY()

	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnAerrowProjectiles();

	UFUNCTION(BlueprintImplementableEvent)
	void HomingTargetDied(AActor* DeadActor);

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Aerrow")
	float ProjectileSpread = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aerrow")
	int32 MaxNumProjectiles = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Aerrow")
	int32 NumProjectiles = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Aerrow")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Aerrow")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Aerrow")
	bool bLaunchHomingProjectiles = true;

	AActor* HomingTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAerowProjectileClass> AerowProjectileClass;

};
