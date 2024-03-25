// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizInputConfig.h"



const UInputAction* USusWizInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                     bool bLogNotFound) const
{

	for (const FSusWizInputAction& IAction : AbilityInputActions)
	{
		if (IAction.InputAction && IAction.InputTag == InputTag)
		{
			return IAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
	
}
