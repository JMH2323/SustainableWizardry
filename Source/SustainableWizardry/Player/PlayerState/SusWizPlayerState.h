// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "SusWizPlayerState.generated.h"

/**
 * 
 */

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	

public:
	
	ASusWizPlayerState();
	
protected:

	// Set ASC and Attributes
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


	
public:
	
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const
	{
		UE_LOG(LogTemp, Warning, TEXT("Player state is getting ABS."));
		return AttributeSet;
	}

	// 13.1 To replicate anything, like the level, we need to override replication here too.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 13.2
	// Force in line tells unreal that whenever this function is called, it will try to use this instance
	// of the function. It can make the code run faster, as it doesn't have to call the function each time
	// but is mostly just good practice.
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

protected:

	// 13
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	
};
