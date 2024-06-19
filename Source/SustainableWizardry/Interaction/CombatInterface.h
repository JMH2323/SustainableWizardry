// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SustainableWizardry/GAS/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UAnimMontage;
class SUSTAINABLEWIZARDRY_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	
	virtual FVector GetCombatSocketLocation();
	virtual FVector GetSecCombatSocketLocation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die(const FVector& DeathImpulse) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInLHydroBeamLoop(bool bInLoop);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInRHydroBeamLoop(bool bInLoop);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHydroBeamLoopEnd(bool bEndLoop);
	
};
