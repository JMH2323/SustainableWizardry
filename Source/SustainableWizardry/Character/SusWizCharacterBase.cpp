// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizCharacterBase.h"

// Sets default values
ASusWizCharacterBase::ASusWizCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* ASusWizCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;	
}

// Called when the game starts or when spawned
void ASusWizCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


