// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizEffectActor.h"
#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"




// Sets default values
ASusWizEffectActor::ASusWizEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}

// Called when the game starts or when spawned
void ASusWizEffectActor::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void ASusWizEffectActor::OnOverlap(AActor* TargetActor)
{

	// Instant Case
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	// Duration Case
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	// Infinite Case. Refer to 3.1
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	
}

void ASusWizEffectActor::OnEndOverlap(AActor* TargetActor)
{

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	// Infinite Case
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	// Infinite Case
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// We want to check this component against our map. So we send our components to the effect.
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;


		// 4.1 Since we can't remove from a loop we are currently end (crash), we make an array.
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		// 4.0
		for (auto HandlePair : ActiveEffectHandles)
		{
			// Check the value that is ending overlap with the infinite value we stored.
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		// 4.2
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
		
	}
	
}

void ASusWizEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{

	// IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Target);
	//
	// if (ASCInterface)
	// {
	//
	// 	ASCInterface->GetAbilitySystemComponent();

	// This!! Does That^^
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;


	check(GameplayEffectClass);
	// Make handle for Specs
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	// Added source to demonstrate Context
	EffectContextHandle.AddSourceObject(this);
	// Make Specs for effect
	// NO LEVEL PT 1 :: FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	
	// Apply // Syntax Nightmare
	//TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// 3.2
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	// 3.1
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	// Check if the application is infinite and if the infinity can end.
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// refer to map made to match spec to handles. 3.3
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}
