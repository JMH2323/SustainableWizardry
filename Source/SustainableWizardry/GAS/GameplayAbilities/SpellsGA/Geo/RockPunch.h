// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SpellsGA/SusWizProjectileSpell.h"
#include "RockPunch.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API URockPunch : public USusWizProjectileSpell
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void FindAndSpawnGeoProjectile();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RockOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PitchOverride;

	const void SpawnGeoProjectile(TSubclassOf<ASusWizProjectiles> GeoProjectile);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASusWizProjectiles> GeoProjectileOne;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASusWizProjectiles> GeoProjectileTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASusWizProjectiles> GeoProjectileThree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASusWizProjectiles> GeoProjectileFour;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASusWizProjectiles> GeoProjectileFive;
	
	
};
