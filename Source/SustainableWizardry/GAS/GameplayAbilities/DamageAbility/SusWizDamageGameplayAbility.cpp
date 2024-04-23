// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"

void USusWizDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{

	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	FGameplayTag Tag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Damage"));
	float DMG = Damage.GetValue();
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Tag, DMG);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}
