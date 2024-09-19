// Fill out your copyright notice in the Description page of Project Settings.


#include "AerrowSpell.h"

#include "SustainableWizardry/SustainableWizardry.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/GAS/GameplayAbilities/Spells/Aero/AerowProjectileClass.h"
#include "SustainableWizardry/GAS/GameplayAbilities/Spells/SpellsBase/SusWizProjectiles.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"

FString UAerrowSpell::GetDescription(int32 Level)
{
	return Super::GetDescription(Level);
}

FString UAerrowSpell::GetNextLevelDescription(int32 Level)
{
	return Super::GetNextLevelDescription(Level);
}

void UAerrowSpell::SpawnAerrowProjectiles()
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
		int32 EffectiveNumProjectiles = 1;
		if (GetAbilityLevel() >= 4)
		{
			EffectiveNumProjectiles = 2;	
		}
		
		// Pass those into getting our rotations and directions for multiple projectiles.
		TArray<FRotator> Rotations = USusWizAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

		// Loop through all possible projectile spawns.
		for (const FRotator& Rot : Rotations)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rot.Quaternion());

			AAerowProjectileClass* Projectile = GetWorld()->SpawnActorDeferred<AAerowProjectileClass>(
			AerowProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
			
			Projectile->NumAeroBounces = GetAbilityLevel() + 1;
			
			if (Projectile->ProjectileMovement)
			{
				if (HomingTarget)
				{
					Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
					Projectile->ProjectileMovement->bIsHomingProjectile = true;
					if (ICombatInterface* CIHM = Cast<ICombatInterface>(HomingTarget))
					{
						if (!CIHM->GetOnDeathDelegate().IsAlreadyBound(this, &UAerrowSpell::HomingTargetDied))
						{
							CIHM->GetOnDeathDelegate().AddDynamic(this, &UAerrowSpell::HomingTargetDied);
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
