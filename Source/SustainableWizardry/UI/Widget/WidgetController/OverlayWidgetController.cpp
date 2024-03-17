// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"


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
	
	

	// 7.1 We want the ability system components tags so we cast to it
	// TODO: Uncomment from 67 to 93
	// Cast<USusWizAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
	// 	// Lambda to use over callback function. Lambda allows us to build and use a function right now.
	// 	[this](const FGameplayTagContainer& AssetTags)
	// 	{
	// 		// 7.2 Use same log message to check tags being broadcast
	// 		for (const FGameplayTag& Tag : AssetTags)
	// 		{
	//
	// 			// Create a way to match the tag with message tags so that only messages print.
	// 			//				FGameplayTag::MatchesTag()
	// 			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
	// 			if (Tag.MatchesTag(MessageTag))
	// 			{
	// 				// How do we get a table and tag and return the row? We should make a function -> GetDataTableRowByTag
	// 			// LAMBDA doesn't know getdatatable function exists so we have to "capture" this object in the []
	// 			const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
	// 			// Pt2
	// 			MessageWidgetRowDelegate.Broadcast(*Row);
	// 			}
	// 			
	// 			// TODONE: TIE INTO TO WIDGET CONTROLLER
	// 			//const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
	// 			//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, Msg);
	// 			
	// 		}
	// 	}
	// );

	
}


// PRE LAMBDA 
// void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
// {
// 	OnHealthChanged.Broadcast(Data.NewValue);
// }