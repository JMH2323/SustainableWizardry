// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "SustainableWizardry/SusWizGameplayTags.h"

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
	
	/* Vital */
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Health, GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Energy, GetEnergyAttribute);
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
			ACharacter* SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
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

void USusWizAttributeSet::EmptyPartOne(const FGameplayEffectModCallbackData& Data)
{


	// PART 1
	//		Get data from Effect Spec and context. 
	const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
	//		Source = causer of effect, Target = target of the effect (OWNER OF THIS AS).
	const UAbilitySystemComponent* SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// We're going to access a lot of pointers so we need checks.
	if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		// If we have an actor who did this, get the player if its a player.
		AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
		const AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();

		// If its not a player, but it is an actor, get the pawn (Enemy).
		if (SourceController == nullptr && SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(SourceAvatarActor))
			{
				SourceController = Pawn->GetController();
			}
		}
		// Get the character as well if it is a player.
		if (SourceController)
		{
			ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
		}
	}

	// First round of data collection complete.
	//  Second round is getting the target. Remember to check pointers!
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		// Set Actor and get it's controller
		AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetAvatarActor);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarActor);
	}
	
}


void USusWizAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

	// Data gives us data about every entity involved in the effect and is VERY powerful.
	Super::PostGameplayEffectExecute(Data);
		
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

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
	// TODO: JEFF AND ALEX

	
	// Vital
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Health, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	
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

void USusWizAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, MaxHealth, OldMaxHealth);
}

void USusWizAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, MaxHealth, OldMaxEnergy);
}

//TODO: JEFF ALEX

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



