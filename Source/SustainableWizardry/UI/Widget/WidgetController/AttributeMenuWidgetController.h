// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SusWizWidgetController.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */

class UAttributeInfo;
struct FSusWizAttributeInfo;
struct FGameplayTag;
struct FGameplayAttribute;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FSusWizAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class SUSTAINABLEWIZARDRY_API UAttributeMenuWidgetController : public USusWizWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

	void BruteForceBroadcast() const;
};
