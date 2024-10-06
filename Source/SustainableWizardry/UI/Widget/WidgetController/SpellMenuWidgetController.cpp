// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellMenuWidgetController.h"

#include "NativeGameplayTags.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Data/AbilityInfo.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetSusWizPS()->GetSpellPoints());
	
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetSusWizASC()->AbilityStatusChanged.AddLambda([this] (const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{


		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetSusWizASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		}
		
		if(AbilityInfo)
		{
			FSusWizAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	
	GetSusWizASC()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
	GetSusWizPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChanged.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;

		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		GetSusWizASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});
	
	
}


void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	
	// If we select a different ability, stop the "equip" action
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityTypeTag;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const FSusWizGameplayTags GameplayTags = FSusWizGameplayTags::Get();	
	const int32 SpellPoints = GetSusWizPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;	

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetSusWizASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetSusWizASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetSusWizASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{

	if (GetSusWizASC())
	{
		GetSusWizASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
	
}

void USpellMenuWidgetController::GlobeDeselect()
{

	// If we select a different ability, stop the "equip" action
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	SelectedAbility.Ability = FSusWizGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FSusWizGameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;

	// Broadcast ability type. Inform user we are waiting for an equip selection.
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
	const FGameplayTag SelectedStatus = GetSusWizASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FSusWizGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetSusWizASC()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{

	if (!bWaitingForEquipSelection) return;
	// Check selected ability against the slot's ability type.
	// (don't equip a passive spell)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetSusWizASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
	
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{

	bWaitingForEquipSelection = false;

	const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();

	FSusWizAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FSusWizAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityTypeTag);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag, PreviousSlot);
	GlobeDeselect();
	
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FSusWizGameplayTags GameplayTags = FSusWizGameplayTags::Get();

	
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
