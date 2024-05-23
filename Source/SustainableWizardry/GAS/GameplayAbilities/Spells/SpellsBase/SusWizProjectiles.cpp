// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizProjectiles.h"

#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/SustainableWizardry.h"


// Sets default values
ASusWizProjectiles::ASusWizProjectiles()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//Sphere->IgnoreActorWhenMoving(GetOwner(), true);
	

	 


	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}


void ASusWizProjectiles::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASusWizProjectiles::OnSphereOverlap);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void ASusWizProjectiles::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	bHit = true;
}

void ASusWizProjectiles::Destroyed()
{
	if (!bHit && !HasAuthority()) OnHit();
	Super::Destroyed();
}


void ASusWizProjectiles::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									  bool bFromSweep, const FHitResult& SweepResult)
{

	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return;
	if (!USusWizAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;
	if (!bHit) OnHit();
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = this->GetActorForwardVector();
			DamageEffectParams.DeathImpulse = DeathImpulse;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			USusWizAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		Destroy();
	}
	else bHit = true;
	
}



