// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SusWizCharacterBase.generated.h"


// Forward declare for GAS
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;


UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASusWizCharacterBase();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
