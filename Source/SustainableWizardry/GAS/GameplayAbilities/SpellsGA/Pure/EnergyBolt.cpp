// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyBolt.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SustainableWizardry/SustainableWizardry.h"
#include "SustainableWizardry/GAS/GameplayAbilities/Spells/SpellsBase/SusWizProjectiles.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"


FString UEnergyBolt::GetDescription(int32 Level)
{

	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ENERGY BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// Energy Cost
			"<Small>Energy Cost: </><Energy>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Launches a bolt of  energy, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</><Default> pure damage</>"),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ENERGY BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Energy Cost: </><Energy>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Launches %d bolts of energy, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</><Default> pure damage</>"),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			FMath::Min(Level, MaxNumProjectiles),
			ScaledDamage);

	}
	
}

FString UEnergyBolt::GetNextLevelDescription(int32 Level)
{
	// Get Damage
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	// Get Cost
	const float EnergyCost = FMath::Abs(GetEnergyCost(Level));
	// Get Cooldown
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			// Title
			"<Title>ENERGY BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Energy Cost: </><Energy>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Launches %d bolts of energy, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</><Default> pure damage</>"),

			// Values
			Level,
			EnergyCost,
			Cooldown,
			FMath::Min(Level, MaxNumProjectiles),
			ScaledDamage);

}

void UEnergyBolt::SpawnProjectiles()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (StoredActorInfo && CombatInterface)
	{
		FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		if (isLeftHanded)
		{
			SocketLocation = CombatInterface->GetSecCombatSocketLocation();
		}
		
		// Getting the player's viewpoint.
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotator;
		StoredActorInfo->PlayerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotator);

		// Get the forward vector of the player viewpoint
		FVector ViewPointForward = PlayerViewPointRotator.Vector();

		// Create a line trace from the viewport location in the direction that the viewport is facing.
		FVector EndPoint = PlayerViewPointLocation + (ViewPointForward * 50000);  // is your trace distance

		FHitResult TraceResult;
		FCollisionQueryParams TraceParams;
		bool bHit = GetWorld()->LineTraceSingleByChannel(TraceResult, PlayerViewPointLocation, EndPoint, ECC_PlayerProjectileTrace, TraceParams);
		// Debug line to draw the line trace.
		//DrawDebugLine(GetWorld(), PlayerViewPointLocation, TraceResult.bBlockingHit ? TraceResult.ImpactPoint : EndPoint, FColor::Green, false, 5.0f, 0, 3.0f);

		// The direction in which the projectile should be shot.
		FVector ShotDirection;
	
		if(bHit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Line trace hit %s"), *TraceResult.GetActor()->GetName());
			// If the line trace hits something, shoot in the direction of the hit.
			ShotDirection = (TraceResult.ImpactPoint - SocketLocation).GetSafeNormal();
			// Check if the hit object is a possible homing target
			if (TraceResult.GetActor() && TraceResult.GetActor()->Implements<UCombatInterface>())
			{
				HomingTarget = TraceResult.GetActor();
			}
		}
		else
		{
			// If the line trace does not hit anything, shoot in the direction of the line trace end point.
			ShotDirection = (EndPoint - SocketLocation).GetSafeNormal();
		}

		FRotator Rotation = ShotDirection.Rotation();
		const FVector Forward = Rotation.Vector();
		// Determine the number of projectiles to spawn based on level.
		const int32 EffectiveNumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
		// Pass those into getting our rotations and directions for multiple projectiles.
		TArray<FRotator> Rotations = USusWizAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

		// Loop through all possible projectile spawns.
		for (const FRotator& Rot : Rotations)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rot.Quaternion());

			ASusWizProjectiles* Projectile = GetWorld()->SpawnActorDeferred<ASusWizProjectiles>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

			if (Projectile->ProjectileMovement)
			{
				if (HomingTarget)
				{
					Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
					Projectile->ProjectileMovement->bIsHomingProjectile = true;
					if (ICombatInterface* CIHM = Cast<ICombatInterface>(HomingTarget))
					{
						if (!CIHM->GetOnDeathDelegate().IsAlreadyBound(this, &UEnergyBolt::HomingTargetDied))
						{
							CIHM->GetOnDeathDelegate().AddDynamic(this, &UEnergyBolt::HomingTargetDied);
						}
					}
				}
				else
				{
					Projectile->ProjectileMovement->bIsHomingProjectile = false;
				}
			}
			Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
			Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
