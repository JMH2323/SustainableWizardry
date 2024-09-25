// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/Spells/SpellsBase/SusWizProjectiles.h"
#include "AerowProjectileClass.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API AAerowProjectileClass : public ASusWizProjectiles
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "AerowProjectile")
	int NumAeroBounces = 5;

protected:

	virtual void BeginPlay() override;
	virtual void OnHit() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	
	
	int BounceCount;
	bool bHitMe;
	bool bHitMeSpawn;

	UFUNCTION(BlueprintCallable)
	virtual void AssignNewHomingTarget();

	UPROPERTY(BlueprintReadWrite, Category = "AerowProjectile")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category = "AerowProjectile")
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, Category = "AerowProjectile")
	TObjectPtr<UNiagaraSystem> AerowTrailEffect;

	UPROPERTY(EditAnywhere, Category = "AerowProjectile")
	TObjectPtr<UNiagaraSystem> AerowFinalImpactEffect;
	
};
