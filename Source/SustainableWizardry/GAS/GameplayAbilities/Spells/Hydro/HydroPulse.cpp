// Fill out your copyright notice in the Description page of Project Settings.


#include "HydroPulse.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"

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
		1000.f,
		TraceHitActor->GetActorLocation());

	// Set the number of targets and get the closest ones, returning closest actors.
	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel(), MaxNumShockTargets);
	USusWizAbilitySystemLibrary::GetClosestTargets(NumAdditionalTargets,
		OverlappingActors,  OutAdditionalTargets, TraceHitActor->GetActorLocation());
}
