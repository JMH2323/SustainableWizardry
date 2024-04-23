// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeInfo.h"

#include "SustainableWizardry/SusWizLogChannels.h"

FSusWizAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{

	// Find ability info in our array for an attribute that shares the attribute tag.
	for (const FSusWizAttributeInfo& Info : AttributeInformation)
	{
		{
			if (Info.AttributeTag.MatchesTagExact(AttributeTag))
			{
				return Info;
			}
		}
	}

	if (bLogNotFound)
	{
		// Print an error if we can't find anything in the array
		UE_LOG(LogSusWiz, Error, TEXT("Cant find Info for AttributeTag [%s] on Attribute Info [%s]."),
		*AttributeTag.ToString(), *GetNameSafe(this));
		
	}

	// If error, return blank info.
	return FSusWizAttributeInfo();
	
}
