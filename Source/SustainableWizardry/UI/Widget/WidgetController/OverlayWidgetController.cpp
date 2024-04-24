// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "SustainableWizardry/GAS/Data/AbilityInfo.h"
#include "SustainableWizardry/GAS/Data/LevelUpInfo.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	const USusWizAttributeSet* SusWizAttributeSet = CastChecked<USusWizAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(SusWizAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(SusWizAttributeSet->GetMaxHealth());
	OnEnergyChanged.Broadcast(SusWizAttributeSet->GetEnergy());
	OnMaxEnergyChanged.Broadcast(SusWizAttributeSet->GetMaxEnergy());
	
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{

	ASusWizPlayerState* SusWizPlayerState = CastChecked<ASusWizPlayerState>(PlayerState);
	SusWizPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

	SusWizPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{OnPlayerLevelChangedDelegate.Broadcast(NewLevel);}
	);
	
	const USusWizAttributeSet* SusWizAttributeSet = CastChecked<USusWizAttributeSet>(AttributeSet);

	// PRE LAMBDA. We bind the callbacks to a function to broadcast delegate changes.
	//  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	// 	SusWizAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	// POST LAMBDA. We can simplify use of expressions by doing the broadcasting as we get the attribute
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		SusWizAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
		);

	// 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		SusWizAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		SusWizAttributeSet->GetEnergyAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnEnergyChanged.Broadcast(Data.NewValue);
		}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		SusWizAttributeSet->GetMaxEnergyAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxEnergyChanged.Broadcast(Data.NewValue);
		}
		);
	
	


	if (USusWizAbilitySystemComponent* SusWizASC = Cast<USusWizAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (SusWizASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(SusWizASC);
		}
		else
		{
			SusWizASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}

		SusWizASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// For example, say that Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}

}

 void UOverlayWidgetController::OnInitializeStartupAbilities(USusWizAbilitySystemComponent* SusWizAbilitySystemComponent)
 {
 	//TODO Get information about all given abilities, look up their Ability Info, and broadcast it to widgets.
 	if (!SusWizAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, SusWizAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		//TODO need a way to figure out the ability tag for a given ability spec.
		FSusWizAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(SusWizAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = SusWizAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	SusWizAbilitySystemComponent->ForEachAbility(BroadcastDelegate);


 }


void UOverlayWidgetController::OnXPChanged(int32 NewXP) const 
{
	const ASusWizPlayerState* SusWizPlayerState = CastChecked<ASusWizPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = SusWizPlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out SusWizPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}




//
// 	
// 	// 7.1 We want the ability system components tags so we cast to it
// 	// TODO: Uncomment from 67 to 93
// 	Cast<USusWizAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
// 		// Lambda to use over callback function. Lambda allows us to build and use a function right now.
// 		[this](const FGameplayTagContainer& AssetTags)
// 		{
// 			// 7.2 Use same log message to check tags being broadcast
// 			for (const FGameplayTag& Tag : AssetTags)
// 			{
// 	
// 				// Create a way to match the tag with message tags so that only messages print.
// 				//				FGameplayTag::MatchesTag()    
// 				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
// 				if (Tag.MatchesTag(MessageTag))
// 				{
// 					// How do we get a table and tag and return the row? We should make a function -> GetDataTableRowByTag
// 				// LAMBDA doesn't know getdatatable function exists so we have to "capture" this object in the []
// 				const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
// 				// Pt2
// 				MessageWidgetRowDelegate.Broadcast(*Row);
// 				}
// 				
// 				// TODONE: TIE INTO TO WIDGET CONTROLLER
// 				//const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
// 				//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, Msg);
// 				
// 			}
// 		}
// 	);
//
// 	
// }


// PRE LAMBDA 
// void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
// {
// 	OnHealthChanged.Broadcast(Data.NewValue);
// }