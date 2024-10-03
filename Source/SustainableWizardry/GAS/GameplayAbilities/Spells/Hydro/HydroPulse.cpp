// Fill out your copyright notice in the Description page of Project Settings.


#include "HydroPulse.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"

FString UHydroPulse::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	EnergyCost *= 10;
	const float Cooldown = GetCooldown(Level);
	int32 NumAdd = FMath::Min(Level + 1, 6);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>HYDRO PULSE</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Energy Cost
			"<Small>Energy Cost: </><Energy>%.1f per second</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>After tracking an enemy, Shoot out a beam of water that"
			" connects and deals </>"

			// Damage
			"<Damage>1.5</><Default> Hydro damage every tenth second. The Beam will also spread to"
			// Additional Targets
			" </><Cooldown>%i</> targets near the enemy."),
			
			// Values
			Level,
			EnergyCost,
			Cooldown,
			NumAdd);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>HYDRO PULSE</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Energy Cost
			"<Small>Energy Cost: </><Energy>%.1f per second</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>After tracking an enemy, Shoot out a beam of water that"
			" connects and deals </>"

			// Damage
			"<Damage>%d</><Default> Hydro damage every tenth second. The Beam will also spread to"
			// Additional Targets
			" </><Cooldown>%i</> targets near the enemy."),
			
			// Values
			Level,
			EnergyCost,
			Cooldown,
			ScaledDamage,
			NumAdd);
	}
	
}

FString UHydroPulse::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	EnergyCost *= 10;
	const float Cooldown = GetCooldown(Level);
	int32 NumAdd = FMath::Min(Level + 1, 6);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>HYDRO PULSE</>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// Energy Cost
		"<Small>Energy Cost: </><Energy>%.1f per second</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		"<Default>After tracking an enemy, Shoot out a beam of water that"
		" connects and deals </>"

		// Damage
		"<Damage>%d</><Default> Hydro damage every tenth second. The Beam will also spread to"
		// Additional Targets
		" </><Cooldown>%i</> targets near the enemy."),
			
		// Values
		Level,
		EnergyCost,
		Cooldown,
		ScaledDamage,
		NumAdd);
}

void UHydroPulse::StoreTraceDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		TraceHitLocation = HitResult.ImpactPoint;
		TraceHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UHydroPulse::StoreOwnerPlayerController()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UHydroPulse::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	// If the Owner Character has the combat interface
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		// Check if the beam is coming from the right hand
		if (!isBeamLeftHanded)
		{
			// Get the beam's origin point (weapon) and make a new update traces after initial hit.
			if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetMainWeapon(OwnerCharacter))
			{
				// Ignore owner for tracing (should be already done in trace channel, extra protection)
				TArray<AActor*> ActorsToIgnore;
				ActorsToIgnore.Add(OwnerCharacter);
				
				FHitResult HitResult;
				const FVector SocketLocation = Weapon->GetSocketLocation(FName("Muzzle_01"));
				UKismetSystemLibrary::SphereTraceSingle(
					OwnerCharacter,
					SocketLocation,
					BeamTargetLocation,
					15.f,
					TraceTypeQuery1,
					false,
					ActorsToIgnore,
					EDrawDebugTrace::None,
					HitResult,
					true);

				// Update hit results for if an enemy walks into new trace
				if (HitResult.bBlockingHit)
				{
					TraceHitLocation = HitResult.ImpactPoint;
					TraceHitActor = HitResult.GetActor();
				}
			}
		}
		// Left handed
		if (isBeamLeftHanded)
		{
			if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetSecWeapon(OwnerCharacter))
			{
				TArray<AActor*> ActorsToIgnore;
				ActorsToIgnore.Add(OwnerCharacter);
				FHitResult HitResult;
				const FVector SocketLocation = Weapon->GetSocketLocation(FName("Muzzle_02"));
				UKismetSystemLibrary::SphereTraceSingle(
					OwnerCharacter,
					SocketLocation,
					BeamTargetLocation,
					15.f,
					TraceTypeQuery1,
					false,
					ActorsToIgnore,
					EDrawDebugTrace::None,
					HitResult,
					true);

				if (HitResult.bBlockingHit)
				{
					TraceHitLocation = HitResult.ImpactPoint;
					TraceHitActor = HitResult.GetActor();
				}
			}
		}
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TraceHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UHydroPulse::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UHydroPulse::PrimaryTargetDied);
		}
	}
	
}

void UHydroPulse::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{

	// Ignore the first hit target and caster.
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	ActorsToIgnore.Add(TraceHitActor);

	// Set storage for nearby actors, use function to get all living characters.
	TArray<AActor*> OverlappingActors;
	USusWizAbilitySystemLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		3000.f,
		TraceHitActor->GetActorLocation());

	// Set the number of targets and get the closest ones, returning closest actors.
	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() + 1, MaxNumShockTargets);
	USusWizAbilitySystemLibrary::GetClosestTargets(NumAdditionalTargets,
		OverlappingActors,  OutAdditionalTargets, TraceHitActor->GetActorLocation());

	for (AActor* Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UHydroPulse::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UHydroPulse::AdditionalTargetDied);
			}
		}
	}
	
}
