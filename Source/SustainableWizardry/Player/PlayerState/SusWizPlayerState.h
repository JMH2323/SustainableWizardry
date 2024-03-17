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



	
public:
	
	
};
