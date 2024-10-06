// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAbilitySystemComponent.h"

#include "AbilitySystemComponent.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/Interaction/PlayerInterface.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SustainableWizardry/SusWizLogChannels.h"
#include "SustainableWizardry/Game/LoadScreenSaveGame.h"
#include "SustainableWizardry/GAS/Data/AbilityInfo.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SusWizGameplayAbility.h"


void USusWizAbilitySystemComponent::AbilityActorInfoSet()
{

	// 6.3 Bind to delegate
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &USusWizAbilitySystemComponent::ClientEffectApplied);

	// 16 testing for getting tags
	const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();
	//GameplayTags.Attributes_Secondary_Armor
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		//FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString()));
	
	
}

void USusWizAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{

	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const USusWizGameplayAbility* SusWizAbility = Cast<USusWizGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(SusWizAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(SusWizAbility->CurrentInputTag);
			SusWizAbility->CurrentInputTag = SusWizAbility->StartupInputTag;
			AbilitySpec.DynamicAbilityTags.AddTag(FSusWizGameplayTags::Get().Abilities_Status_Equipped);
			
			// Give ability to player
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void USusWizAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
		AbilitySpec.DynamicAbilityTags.AddTag(FSusWizGameplayTags::Get().Abilities_Status_Equipped);
	}
}


void USusWizAbilitySystemComponent::AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData)
{
	for (const FSavedAbility& Data : SaveData->SavedAbilities)
	{
		const TSubclassOf<UGameplayAbility> LoadedAbilityClass = Data.GameplayAbility;
		FGameplayAbilitySpec LoadedAbilitySpec = FGameplayAbilitySpec(LoadedAbilityClass, Data.AbilityLevel);
		LoadedAbilitySpec.DynamicAbilityTags.AddTag(Data.AbilitySlot);
		LoadedAbilitySpec.DynamicAbilityTags.AddTag(Data.AbilityStatus);
		if (Data.AbilityType == FSusWizGameplayTags::Get().Abilities_Type_Active)
		{
			GiveAbility(LoadedAbilitySpec);
		}
		else if (Data.AbilityType == FSusWizGameplayTags::Get().Abilities_Type_Passive)
		{
			if (Data.AbilityStatus.MatchesTagExact(FSusWizGameplayTags::Get().Abilities_Status_Equipped))
			{
				GiveAbilityAndActivateOnce(LoadedAbilitySpec);
				//MulticastActivatePassiveEffect(Data.AbilityTag, true);
			}
			else
			{
				GiveAbility(LoadedAbilitySpec);
			}
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void USusWizAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle,
					AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void USusWizAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
				
			}
		}
	}
}

void USusWizAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle,
				AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void USusWizAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogSusWiz, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag USusWizAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}


void USusWizAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
			ServerUpgradeAttribute(AttributeTag);
	}
}


void USusWizAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void USusWizAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	UE_LOG(LogTemp, Warning, TEXT("Server Here: Calling %s"), *AttributeTag.ToString());
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	
}



FGameplayTag USusWizAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag USusWizAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag USusWizAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag USusWizAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag USusWizAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

void USusWizAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);

		const bool bStatusValid = Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_Unlocked;
		if (bStatusValid)
		{
			// Remove this InputTag (slot) from any Ability that has it.
			ClearAbilitiesOfSlot(Slot);
			if (const USusWizGameplayAbility* SusWizAbility = Cast<USusWizGameplayAbility>(AbilitySpec->Ability))
			{
				SusWizAbility->CurrentInputTag = FSusWizGameplayTags::Get().Abilities_None;
			}
			// Clear this ability's slot, just in case it's a different slot
			ClearSlot(AbilitySpec);
			// Now, assign this ability to this slot
			AbilitySpec->DynamicAbilityTags.AddTag(Slot);
			if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Equipped);
			}
			// Update InputTag of the ability
			if (const USusWizGameplayAbility* SusWizAbility = Cast<USusWizGameplayAbility>(AbilitySpec->Ability))
			{
				SusWizAbility->CurrentInputTag = Slot; // Update the InputTag
			}
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
	}
}

void USusWizAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

void USusWizAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
	MarkAbilitySpecDirty(*Spec);
}

void USusWizAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool USusWizAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

FGameplayAbilitySpec* USusWizAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{

	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
	
}

void USusWizAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = USusWizAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FSusWizAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FSusWizGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FSusWizGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

bool USusWizAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{

	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if(USusWizGameplayAbility* SusWizAbility = Cast<USusWizGameplayAbility>(AbilitySpec->Ability))
		{
			if (USusWizAbilitySystemLibrary::isAbilityTagDemo(AbilityTag))
			{
				OutDescription = SusWizAbility->GetDescription(AbilitySpec->Level);
				OutNextLevelDescription = SusWizAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
				return true;
			}
			OutDescription = FString::Printf(TEXT("<Default>Not included in DEMO</>"));
			OutNextLevelDescription = FString::Printf(TEXT("<Default>Not included in DEMO</>"));
			return true;
		}
	}
	const UAbilityInfo* AbilityInfo = USusWizAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (USusWizAbilitySystemLibrary::isAbilityTagDemo(AbilityTag))
	{
		OutDescription = USusWizGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}
	else
	{
		OutDescription = FString::Printf(TEXT("<Default>Not included in DEMO</>"));
	}

	// Check the ability is not "null" or none
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FSusWizGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();
	}
	else
	{
		if (USusWizAbilitySystemLibrary::isAbilityTagDemo(AbilityTag))
		{
			OutDescription = USusWizGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
		}
		else
		{
			OutDescription = FString::Printf(TEXT("<Default>Not included in DEMO</>"));
		}
	}
	
	OutNextLevelDescription = FString();
	return false;
	
}

void USusWizAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{

		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		
		const FSusWizGameplayTags GameplayTags = FSusWizGameplayTags::Get();
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Unlocked);
			Status = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) || Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
		{
			AbilitySpec->Level += 1;
		}

		UE_LOG(LogTemp, Warning, TEXT("Reached Tag phase"));
		if(AbilityTag.MatchesTagExact(GameplayTags.Abilities_Aero_Airrow))
		{
			UpgradeAttribute(GameplayTags.Attributes_Primary_Swift);
		}
		if(AbilityTag.MatchesTagExact(GameplayTags.Abilities_Solar_Beam))
		{
			UpgradeAttribute(GameplayTags.Attributes_Primary_Flare);
		}
		if(AbilityTag.MatchesTagExact(GameplayTags.Abilities_Geo_RockPunch))
		{
			UpgradeAttribute(GameplayTags.Attributes_Primary_Seismic);
		}
		if(AbilityTag.MatchesTagExact(GameplayTags.Abilities_Hydro_HydroPulse))
		{
			UE_LOG(LogTemp, Warning, TEXT("Reached HYDRO Tag phase"));
			UpgradeAttribute(GameplayTags.Attributes_Primary_Deep);
		}

		ClientUpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
	
}


void USusWizAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
                                                                             const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}


void USusWizAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                       const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{


	// 6.4 Debug message to check effect applied
	// Can launch in Debug to place Breakpoint and check data
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString("Effect Applied!"));

	// 6.5
	// Want to grab all tags and feed them to our stuff, like the player or, better, the HUD
	// We can't use a TArray for tags, remember we have them already stored in Data. So we use the container
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	// 7
	EffectAssetTags.Broadcast(TagContainer);

	// 6.5 DEBUG TO GET ASSET TAGS THROUGH DEBUG
	// for (const FGameplayTag& Tag : TagContainer)
	// {
	// 	// TODONE: BROADCAST TO WIDGET CONTROLLER
	// 	const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
	// 	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, Msg);
	// }
	
}
