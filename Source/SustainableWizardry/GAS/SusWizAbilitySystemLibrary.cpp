// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAbilitySystemLibrary.h"

#include "GameplayEffectTypes.h"
#include "SustainableWizardry/Game/SuzWizGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/SusWizAbilityTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"
#include "SustainableWizardry/UI/HUD/SusWizHUD.h"



// Function for getting the overlay widget controller and all it's relevant GAS data that we might want to display it 
UOverlayWidgetController* USusWizAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{

	// Get and check the player controller by referring to the world context
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		{
		// Get and check the HUD of the player controller
		if(ASusWizHUD* SusWizHUD = Cast<ASusWizHUD>(PC->GetHUD()))
			{
				// Get the player state from the player controller 
				ASusWizPlayerState* PS = PC->GetPlayerState<ASusWizPlayerState>();
			
				// Get the ASC and Attributes from the player state
				UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
				UAttributeSet* AS = PS->GetAttributeSet();

				// Pass the above data and construct the widget controller overlay with this information so we
				// can refer to it and display it to the player.
				const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
				return SusWizHUD->GetOverlayWidgetController(WidgetControllerParams);
			}
		}

	// if none of the above, return null
	return nullptr;
	
}

UAttributeMenuWidgetController* USusWizAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{

	// Get and check the player controller by referring to the world context
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		// Get and check the HUD of the player controller
		if(ASusWizHUD* SusWizHUD = Cast<ASusWizHUD>(PC->GetHUD()))
		{
			// Get the player state from the player controller 
			ASusWizPlayerState* PS = PC->GetPlayerState<ASusWizPlayerState>();
			
			// Get the ASC and Attributes from the player state
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			// Pass the above data and construct the attribute controller overlay with this information so we
			// can refer to it and display it to the player.
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return SusWizHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	// if none of the above, return null
	return nullptr;

	
}

void USusWizAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	
}

void USusWizAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{

	ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SusWizGameMode == nullptr) return;

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
}

void USusWizAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}


UCharacterClassInfo* USusWizAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SusWizGameMode == nullptr) return nullptr;
	return SusWizGameMode->CharacterClassInfo;
}

bool USusWizAbilitySystemLibrary::IsDodgedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->IsDodgedHit();
	}
	return false;
}

bool USusWizAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->IsCriticalHit();
	}
	return false;
}

void USusWizAbilitySystemLibrary::SetIsDodgedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDodgedHit)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetIsDodgedHit(bInIsDodgedHit);
	}
}

void USusWizAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}
