// Fill out your copyright notice in the Description page of Project Settings.


#include "AerowProjectileClass.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "SustainableWizardry/Character/SusWizEnemy.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"


void AAerowProjectileClass::BeginPlay()
{
	Super::BeginPlay();
	BounceCount = 0;
}

void AAerowProjectileClass::OnHit()
{
	Super::OnHit();
	bHit = false;
	BounceCount++;
	if (BounceCount < NumAeroBounces + 1)
	{
		AssignNewHomingTarget();		
	}
	else Destroy();
}

void AAerowProjectileClass::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Me"));
		bHitMe = true;
		// TODO: Fix Hitting Self
		AssignNewHomingTarget();
		return;
	}
	if (!USusWizAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;
	// Allows for ground collision events. ex: bouncing.
	//if (!OtherActor->ActorHasTag("Enemy")) return;
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// Determine Death impulse from projectile and apply to damage params
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			// Take the Damage effect params and apply the effect to the target, effecting their ASC.
			// ASC is where Damage is handled.
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			USusWizAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
	if (!bHit)
	{
		bHitMe = false;
		OnHit();
	}
}


void AAerowProjectileClass::AssignNewHomingTarget()
{
	if (DamageEffectParams.SourceAbilitySystemComponent && DamageEffectParams.TargetAbilitySystemComponent)
	{
		AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
		AActor* OldTargetAvatarActor = DamageEffectParams.TargetAbilitySystemComponent->GetAvatarActor();
		TArray<AActor*> ActorsToIgnore;
		if (bHitMe)
		{
			ActorsToIgnore.Add(SourceAvatarActor);
		}
		else
		{
			ActorsToIgnore.Add(OldTargetAvatarActor);
		}
		
		TArray<AActor*> OtherLiveActors;
		TArray<AActor*> ClosestTargetArray;
		
		USusWizAbilitySystemLibrary::GetLivePlayersWithinRadius(SourceAvatarActor, OtherLiveActors, ActorsToIgnore, 10000, GetActorLocation());

		// Filter Spawners
		TArray<AActor*> FilteredActors;
		for (AActor* Actor : OtherLiveActors)
		{
			if (Actor->IsPendingKill())
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor Pending kill: %s"), *Actor->GetName());
				continue;
			}
			ASusWizEnemy* Enemy = Cast<ASusWizEnemy>(Actor);
			if(Enemy)
			{
				if (Enemy->CombatTarget == nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Filtering out spawner: %s"), *Actor->GetName());
				}
			}
			else
			{
				FilteredActors.Add(Actor);
			}
		}
		
		USusWizAbilitySystemLibrary::GetClosestTargets(1, FilteredActors, ClosestTargetArray, GetActorLocation());

		if (ClosestTargetArray.Num() > 0)
		{
			if(AActor* NewTargetActor = ClosestTargetArray[0])
			{
				UE_LOG(LogTemp, Warning, TEXT("Old target Was: %s"), *OldTargetAvatarActor->GetName());
				UE_LOG(LogTemp, Warning, TEXT("New target selected: %s"), *NewTargetActor->GetName());

				OldTargetAvatarActor = NewTargetActor;
				if(NewTargetActor == SourceAvatarActor)
				{
					ProjectileMovement->HomingTargetComponent = nullptr;
					ProjectileMovement->HomingTargetComponent = NewTargetActor->GetRootComponent();
				}
				else
				{
					if(NewTargetActor->GetOwner()->GetRootComponent())
					{
						ProjectileMovement->HomingTargetComponent = nullptr;
						ProjectileMovement->HomingTargetComponent = NewTargetActor->GetOwner()->GetRootComponent();
					}
					else
					{
						AssignNewHomingTarget();
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No new target found"));
			Destroy();
		}
	}
		
}



