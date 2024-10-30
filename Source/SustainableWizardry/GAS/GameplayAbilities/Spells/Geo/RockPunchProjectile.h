// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/Spells/SpellsBase/SusWizProjectiles.h"
#include "RockPunchProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API ARockPunchProjectile : public ASusWizProjectiles
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	ARockPunchProjectile();

	
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchBounce")
	bool bCanBounce = true;
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchBounce")
	bool bHasHitEnemy = false;
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchBounce")
	int32 MaxRockBounces = 6;
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchBounce")
	int32 CurrentRockBounces = 0;
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchBounce")
	float BounceVelocityZ = 1.5;
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchBounce")
	float BounceVelocityY = 800;
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchBounce")
	FVector BounceEffectOffset;
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchBounce")
	float BounceImpactRadius = 200;
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchBounce")
	FVector NiagaraScale = FVector(1.0f);
	
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchVariables")
	bool bCanSplash = false;
	UPROPERTY(EditDefaultsOnly, Category = "RockPunchVariables")
	float SplashImpactRadius = 200;
	
protected:

	UPROPERTY(EditAnywhere, Category = "RockPunchBounce")
	TObjectPtr<UNiagaraSystem> RockUpearthEffect;
	
	virtual void OnHit() override;
	UFUNCTION(BlueprintCallable)
	void HandleBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
