// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "SustainableWizardry/Game/SuzWizGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/SusWizAbilityTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"
#include "SustainableWizardry/UI/HUD/SusWizHUD.h"
#include "SustainableWizardry/UI/Widget/WidgetController/SusWizWidgetController.h"


bool USusWizAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
	FWidgetControllerParams& OutWCParams, ASusWizHUD*& OutSusWizHUD)
{
	// Get and check the player controller by referring to the world context
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		// Get and check the HUD of the player controller
		OutSusWizHUD = Cast<ASusWizHUD>(PC->GetHUD());
		if(OutSusWizHUD)
		{
			// Get the player state from the player controller 
			ASusWizPlayerState* PS = PC->GetPlayerState<ASusWizPlayerState>();
			
			// Get the ASC and Attributes from the player state
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			// Pass the above data and construct the attribute controller overlay with this information so we
			// can refer to it and display it to the player.
			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
		
}

	

// Function for getting the overlay widget controller and all it's relevant GAS data that we might want to display it 
UOverlayWidgetController* USusWizAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{

	FWidgetControllerParams WCParams;
	ASusWizHUD* SusWizHUD = nullptr;
	

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SusWizHUD))
	{
		return SusWizHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
	
	
}

UAttributeMenuWidgetController* USusWizAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{

	FWidgetControllerParams WCParams;
	ASusWizHUD* SusWizHUD = nullptr;
	

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SusWizHUD))
	{
		return SusWizHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;

	
}

USpellMenuWidgetController* USusWizAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASusWizHUD* SusWizHUD = nullptr;
	

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SusWizHUD))
	{
		return SusWizHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}


void USusWizAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                              ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	
}

void USusWizAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{

	ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SusWizGameMode == nullptr) return;

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
}


bool USusWizAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

FGameplayEffectContextHandle USusWizAbilitySystemLibrary::ApplyDamageEffect(
	const FDamageEffectParams& DamageEffectParams)
{

	const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Param_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Param_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Param_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Param_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContexthandle;
	
}

void USusWizAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                             TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                             const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}


UCharacterClassInfo* USusWizAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SusWizGameMode == nullptr) return nullptr;
	return SusWizGameMode->CharacterClassInfo;
}

UAbilityInfo* USusWizAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{


	const ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SusWizGameMode == nullptr) return nullptr;
	return SusWizGameMode->AbilityInfo;
	
}

bool USusWizAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float USusWizAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float USusWizAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float USusWizAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag USusWizAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (SusWizEffectContext->GetDamageType().IsValid())
		{
			return *SusWizEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector USusWizAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

void USusWizAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InImpulse)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetDeathImpulse(InImpulse);
	}
}

bool USusWizAbilitySystemLibrary::IsDodgedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->IsDodgedHit();
	}
	return false;
}

bool USusWizAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->IsCriticalHit();
	}
	return false;
}

void USusWizAbilitySystemLibrary::SetIsDodgedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDodgedHit)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetIsDodgedHit(bInIsDodgedHit);
	}
}

void USusWizAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void USusWizAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInSuccessfulDebuff)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void USusWizAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetDebuffDamage(InDamage);
	}
}

void USusWizAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetDebuffDuration(InDuration);
	}
}

void USusWizAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,
	float InFrequency)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void USusWizAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		//SusWizEffectContext->SetDamageType(DamageType);
	}
}

int32 USusWizAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
                                                               ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}
