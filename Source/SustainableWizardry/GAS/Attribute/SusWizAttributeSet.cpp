// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "SustainableWizardry/SusWizLogChannels.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"
#include "SustainableWizardry/Interaction/PlayerInterface.h"
#include "SustainableWizardry/Player/SusWizPlayerController.h"

USusWizAttributeSet::USusWizAttributeSet()
{
	// This is where we CAN set our base values
	//InitHealth(50.0f);
	//InitMaxHealth(100.0f);

const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();

	// Adding Tag to Attribute Conversion
	/* Primary */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Deep, GetDeepAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Flare, GetFlareAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Seismic, GetSeismicAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Swift, GetSwiftAttribute);

	/* Secondary */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxEnergy, GetMaxEnergyAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPen, GetArmorPenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalChance, GetCriticalChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Dodge, GetDodgeAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_DamageScaling, GetDamageScaleAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealingScaling, GetHealingScaleAttribute);
	
	/* Vital */
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Health, GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Energy, GetEnergyAttribute);

	/* Resistance Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Pure, GetPureResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}


// pre attribute change allows us to perform clamping BEFORE the changes are applied.
// Demonstrated in: UE_LOG(LogTemp, Warning, TEXT("Health: %f), NewValue).
// Does not permanently change the modifier, so is not preferred for final clamping.
void USusWizAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);


	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
}

void USusWizAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData Data, FEffectProperties& Props) const
{

	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();

	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	
	// We're going to access a lot of pointers so we need checks.
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		// If we have an actor who did this, get the player if its a player.
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		// If its not a player, but it is an actor, get the pawn (Enemy).
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		// Get the character as well if it is a player.
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	// First round of data collection complete.
	//  Second round is getting the target. Remember to check pointers!
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		// Set Actor and get it's controller
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
	
}




void USusWizAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

	// Data gives us data about every entity involved in the effect and is VERY powerful.
	Super::PostGameplayEffectExecute(Data);
		
	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	if(Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;

	// Now that we have props, we can access all post gameplay effect executions!!

	// Debug for evaluating health clamping
	// if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	// {
	// 	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Yellow, FString::Printf(TEXT("Bruh %f"), GetHealth()));
	// }

	// Clamp twice. Once before effect and once after
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0.f, GetMaxEnergy()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void USusWizAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die(USusWizAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			}
			SendXPEvent(Props);
		}
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FSusWizGameplayTags::Get().Effects_HitReact);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}

		const bool bBlock = USusWizAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		const bool bCriticalHit = USusWizAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		if (USusWizAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
	}
}

void USusWizAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);

	// Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel);
			const int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel);

			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);

			SetHealth(GetMaxHealth());
			SetEnergy(GetMaxEnergy());

			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}

		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void USusWizAttributeSet::Debuff(const FEffectProperties& Props)
{
	// TODO: Add Debuffs
}


void USusWizAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bDodgedHit, bool bCrit)
{

	if(Props.SourceCharacter != Props.TargetCharacter)
	{
		if(ASusWizPlayerController* PC = Cast<ASusWizPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bDodgedHit, bCrit);
			return;
		}
		if(ASusWizPlayerController* PC = Cast<ASusWizPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bDodgedHit, bCrit);
		}
				
	}
	
}

void USusWizAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = USusWizAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}

/*
 * Attribute stuff start
 */

void USusWizAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// All Replicated Attributes

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Flare, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Deep, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Seismic, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Swift, COND_None, REPNOTIFY_Always);
	

	// Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, ArmorPen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Dodge, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, DamageScale, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, HealingScale, COND_None, REPNOTIFY_Always);

	
	// Vital
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Energy, COND_None, REPNOTIFY_Always);

	// Resistance Attributes

	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, PureResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, HydroResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, RockResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, AeroResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, SolarResistance, COND_None, REPNOTIFY_Always);
	
}


/*
 * Primary
 */

void USusWizAttributeSet::OnRep_Flare(const FGameplayAttributeData& OldFlare) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Flare, OldFlare);
}

void USusWizAttributeSet::OnRep_Deep(const FGameplayAttributeData& OldDeep) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Deep, OldDeep);
}

void USusWizAttributeSet::OnRep_Seismic(const FGameplayAttributeData& OldSeismic) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Seismic, OldSeismic);
}

void USusWizAttributeSet::OnRep_Swift(const FGameplayAttributeData& OldSwift) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Swift, OldSwift);
}

/*
 * Secondary
 */

void USusWizAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Armor, OldArmor);
}

void USusWizAttributeSet::OnRep_ArmorPen(const FGameplayAttributeData& OldArmorPen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, ArmorPen, OldArmorPen);
}

void USusWizAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, CriticalChance, OldCriticalChance);
}

void USusWizAttributeSet::OnRep_Dodge(const FGameplayAttributeData& OldDodge) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Dodge, OldDodge);
}

void USusWizAttributeSet::OnRep_DamageScale(const FGameplayAttributeData& OldDamageScale) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, DamageScale, OldDamageScale);
}

void USusWizAttributeSet::OnRep_HealingScale(const FGameplayAttributeData& OldHealingScale) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, HealingScale, OldHealingScale);
}

void USusWizAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, MaxHealth, OldMaxHealth);
}

void USusWizAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, MaxHealth, OldMaxEnergy);
}

/*
 * Vital
 */

void USusWizAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Health, OldHealth);
}

void USusWizAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, MaxHealth, OldEnergy);
}


/*
 * Resistance
 */


void USusWizAttributeSet::OnRep_PureResistance(const FGameplayAttributeData& OldPureResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, PureResistance, OldPureResistance);
}

void USusWizAttributeSet::OnRep_HydroResistance(const FGameplayAttributeData& OldHydroResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, HydroResistance, OldHydroResistance);
}

void USusWizAttributeSet::OnRep_RockResistance(const FGameplayAttributeData& OldRockResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, RockResistance, OldRockResistance);
}

void USusWizAttributeSet::OnRep_AeroResistance(const FGameplayAttributeData& OldAeroResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, AeroResistance, OldAeroResistance);
}

void USusWizAttributeSet::OnRep_SolarResistance(const FGameplayAttributeData& OldSolarResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, SolarResistance, OldSolarResistance);
}

void USusWizAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, PhysicalResistance, OldPhysicalResistance);
}
