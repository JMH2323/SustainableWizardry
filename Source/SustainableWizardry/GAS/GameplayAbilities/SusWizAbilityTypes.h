#pragma once


#include "GameplayEffectTypes.h"

#include "SusWizAbilityTypes.generated.h"


// Gameplay effect types
USTRUCT(BlueprintType)
struct FSusWizGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsDodgedHit () const { return bIsDodgedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsDodgedHit(bool bInIsDodgedHit) { bIsDodgedHit = bInIsDodgedHit; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	bool bIsDodgedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

};

template<>
struct TStructOpsTypeTraits<FSusWizGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FSusWizGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};