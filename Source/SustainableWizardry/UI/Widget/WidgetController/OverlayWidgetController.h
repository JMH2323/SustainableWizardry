// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SusWizWidgetController.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "SustainableWizardry/GAS/Data/AbilityInfo.h"
#include "Engine/DataTable.h"

#include "OverlayWidgetController.generated.h"

// 8.0 Create Row Structure for our Data Tables

struct FSusWizAbilityInfo;
USTRUCT(BlueprintType, Blueprintable)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	// Tag for sending messages to the screen
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class USusWizUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
	
};

class USusWizUserWidget;
class UAbilityInfo;
class USusWizAbilitySystemComponent;

// 7.0
// Here we are declaring a delegate for every attribute. We can scale them instead -> see part 10
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChangedSignature, float, NewEnergy);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxEnergyChangedSignature, float, NewMaxEnergy);

// 10. Single delegate for changing attributes. Make sure to change "changedsignatures" for each later here.
// changing attributes = broadcasting a float. KEEP ATTRIBUTES FLOATS FOR THIS DELEGATE
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

// 9. 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);






/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SUSTAINABLEWIZARDRY_API UOverlayWidgetController : public USusWizWidgetController
{
	GENERATED_BODY()


public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;

	// 7 before 10. We create the change signature for each value
	//UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	//FOnHealthChangedSignature OnHealthChanged;

	// 10
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnEnergyChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxEnergyChanged;

	//9.1 
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;
	
protected:

	// Get the data table for messaging widgets
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	
	
	// PRE LAMBDA
	// Callbacks on delegates. When change is made, broadcast.
	//void HealthChanged(const FOnAttributeChangeData& Data) const;
	

	// Template files want you to create the function here and not in the cpp
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnXPChanged(int32 NewXP);
	
};

// Template files want you to create the function here and not in the cpp
template<typename T>
	T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
	