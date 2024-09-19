// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "SustainableWizardry/Game/SuzWizGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/SusWizAbilityTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "SustainableWizardry/Game/LoadScreenSaveGame.h"
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
	if (CharacterClassInfo == nullptr) return;
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

void USusWizAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();

	const AActor* SourceAvatarActor = ASC->GetAvatarActor();
	FGameplayEffectContextHandle EffectContexthandle = ASC->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContexthandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Deep, SaveGame->Deep);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Flare, SaveGame->Flare);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Seismic, SaveGame->Seismic);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Swift, SaveGame->Swift);

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	// Secondary Attributes
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	// Vital Attributes
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	
}


bool USusWizAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	if(FirstActor && SecondActor)
	{
		const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
		const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
		const bool bFriends = bBothArePlayers || bBothAreEnemies;
		return !bFriends;
	}
	else
	{
		return true;
	}
	
}

void USusWizAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors,
	TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	// If there are less actors than max targets, set all targeting to the close actors.
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		// If there are no more nearby actors, stop.
		if (ActorsToCheck.Num() == 0) break;
		
		// Set the closest distance to large as a start and keep closest actor info
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;

		// Loop through all potential targets in range, check distance and add closest ones to targeting.
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		// After loop, remove the closest found actor and store it as a unique return.
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

FGameplayEffectContextHandle USusWizAbilitySystemLibrary::ApplyDamageEffect(
	const FDamageEffectParams& DamageEffectParams)
{

	const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	
	SetDeathImpulse(EffectContexthandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContexthandle, DamageEffectParams.KnockbackForce);

	// Radial Damage Mods
	SetIsRadialDamage(EffectContexthandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContexthandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContexthandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContexthandle, DamageEffectParams.RadialDamageOrigin);
	
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

bool USusWizAbilitySystemLibrary::isInputTagLeftHanded(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FSusWizGameplayTags::Get().InputTag_LMB)) return true;
	if (InputTag.MatchesTagExact(FSusWizGameplayTags::Get().InputTag_1)) return true;
	if (InputTag.MatchesTagExact(FSusWizGameplayTags::Get().InputTag_3)) return true;
	return false;
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

FVector USusWizAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{

	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
	
}

bool USusWizAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->IsRadialDamage();
	}
	return false;
}

float USusWizAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float USusWizAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector USusWizAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<const FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SusWizEffectContext->GetRadialDamageOrigin();
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

void USusWizAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InForce)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetKnockbackForce(InForce);
	}
}

void USusWizAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void USusWizAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InInnerRadius)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void USusWizAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InOuterRadius)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void USusWizAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InOrigin)
{
	if (FSusWizGameplayEffectContext* SusWizEffectContext = static_cast<FSusWizGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SusWizEffectContext->SetRadialDamageOrigin(InOrigin);
	}
}

TArray<FRotator> USusWizAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis,
                                                                   float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> USusWizAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis,
	float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
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
