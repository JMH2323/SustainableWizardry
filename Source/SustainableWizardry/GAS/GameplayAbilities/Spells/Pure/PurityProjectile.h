// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/Spells/SpellsBase/SusWizProjectiles.h"
#include "PurityProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API APurityProjectile : public ASusWizProjectiles
{
	GENERATED_BODY()



	
protected:
	
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	
};
