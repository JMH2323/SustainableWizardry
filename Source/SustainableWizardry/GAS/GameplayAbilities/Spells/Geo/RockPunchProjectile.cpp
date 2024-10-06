// Fill out your copyright notice in the Description page of Project Settings.


#include "RockPunchProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"

// Sets default values
ARockPunchProjectile::ARockPunchProjectile()
{
	
	// For RockPunch, enable gravity and the ability to bounce
	ProjectileMovement->ProjectileGravityScale = 1.f;  // Enable gravity to allow bouncing
	ProjectileMovement->bShouldBounce = true;
	bCanBounce = true; 
	ProjectileMovement->Bounciness = 0.6f;  // bounce height
	ProjectileMovement->Friction = 0.9f;  // friction
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 10.0f;

	// Bind the OnProjectileBounce event to a custom function
	ProjectileMovement->OnProjectileBounce.AddDynamic(this, &ARockPunchProjectile::HandleBounce);

}

void ARockPunchProjectile::HandleBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (bCanBounce)
	{
		// Modify the bounce velocity to achieve a higher angle, simulates the "earth" pushing the rock back
		FVector NewBounceVelocity = ImpactVelocity;
		
		// Increase for higher bounce angle
		NewBounceVelocity.Z = FMath::Max(NewBounceVelocity.Z * BounceVelocityZ, BounceVelocityY);
		
		ProjectileMovement->Velocity = NewBounceVelocity;

		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		FVector SystemOffset = GetActorLocation();
		SystemOffset = SystemOffset + BounceEffectOffset;
		
		// Rock upearth niagara effect
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, RockUpearthEffect, SystemOffset,
			FRotator::ZeroRotator, NiagaraScale);
		
		// Apply damage to all enemies near to the bounce.
		if (HasAuthority() && bCanSplash)
		{
			TArray<AActor*> ActorsToIgnore;
			AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
			ActorsToIgnore.Add(SourceAvatarActor);
			TArray<AActor*> OverlappingActors;
			USusWizAbilitySystemLibrary::GetLivePlayersWithinRadius(
			this,
			OverlappingActors,
			ActorsToIgnore,
			BounceImpactRadius,
			ImpactResult.Location);
			for (AActor* Target : OverlappingActors)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
				{
					if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
					{
						// Determine Death impulse from projectile and apply to damage params
						const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
						DamageEffectParams.DeathImpulse = DeathImpulse;

						// Determine knockback chance and apply to damage params
						const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
						if (bKnockback)
						{
							FRotator Rotation = GetActorRotation();
							Rotation.Pitch = 35.f;

							const FVector KnockbackDirection = Rotation.Vector();
							const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
							DamageEffectParams.KnockbackForce = KnockbackForce;
						}

						// Take the Damage effect params and apply the effect to the target, effecting their ASC.
						// ASC is where Damage is handled.
						DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
						USusWizAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
					}
				}
			}
		}
		CurrentRockBounces++;
		
		// Scale the rock to shrink with bounces
		if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())))
		{
			FVector NewScale = MeshComp->GetComponentScale() * 0.8f; // Shrink
			MeshComp->SetWorldScale3D(NewScale);
			
			NiagaraScale *= 0.8f;
		}
		// Scale collision
		if (USphereComponent* CollisionComp = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass())))
		{
			float NewRadius = CollisionComp->GetUnscaledSphereRadius() * 0.8f;  // Shrink the collision sphere by 20%
			CollisionComp->SetSphereRadius(NewRadius, true);  // Adjust the sphere radius and update physics
		}		
		// Reduce bounce and splash impact radii % as well
		BounceImpactRadius *= 0.9f;
		SplashImpactRadius *= 0.9f;
		// Now that the projectile has bounced, it can no longer bounce, unless it has yet to hit an enemy
		// or has bounced the max number of times.
		if (bHasHitEnemy || CurrentRockBounces >= MaxRockBounces)
		{
			bCanBounce = false;
		}
		
	}
	else
	{
		if (!bHit) OnHit();
		if (HasAuthority()) Destroy();
	}
}




void ARockPunchProjectile::OnHit()
{
	
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator,
		FVector(1.f), true);
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (!bCanBounce)
	{
		bHit = true;
	}
	
}

void ARockPunchProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									  bool bFromSweep, const FHitResult& SweepResult)
{

	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return;
	if (!USusWizAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;
	
	// Non-Enemy collision events. ex: bouncing.
	if (!OtherActor->ActorHasTag("Enemy") && bCanBounce) return;
	bHasHitEnemy = true;
	if (!bHit) OnHit();
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// Determine Death impulse from projectile and apply to damage params
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			// Determine knockback chance and apply to damage params
			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 35.f;

				const FVector KnockbackDirection = -Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}

			// Take the Damage effect params and apply the effect to the target, effecting their ASC.
			// ASC is where Damage is handled.
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			USusWizAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		// If we have splash damage enabled, also damage all nearby enemies
		if (bCanSplash)
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(SourceAvatarActor);
			ActorsToIgnore.Add(OtherActor);
			TArray<AActor*> OverlappingActors;
			USusWizAbilitySystemLibrary::GetLivePlayersWithinRadius(
			this,
			OverlappingActors,
			ActorsToIgnore,
			SplashImpactRadius,
			OtherActor->GetActorLocation());
			for (AActor* Target : OverlappingActors)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
				{
					if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
					{
						// Determine Death impulse from projectile and apply to damage params
						const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
						DamageEffectParams.DeathImpulse = DeathImpulse;

						// Determine knockback chance and apply to damage params
						const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
						if (bKnockback)
						{
							FRotator Rotation = GetActorRotation();
							Rotation.Pitch = 35.f;

							const FVector KnockbackDirection = Rotation.Vector();
							const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
							DamageEffectParams.KnockbackForce = KnockbackForce;
						}

						// Take the Damage effect params and apply the effect to the target, effecting their ASC.
						// ASC is where Damage is handled.
						DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
						USusWizAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
					}
				}
			}
		}
		
		if (!bCanBounce)
		{
			Destroy();
		}
	}
	else bHit = true;
	
}


