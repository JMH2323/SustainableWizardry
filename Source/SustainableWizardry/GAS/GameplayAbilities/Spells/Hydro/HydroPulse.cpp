// Fill out your copyright notice in the Description page of Project Settings.


#include "HydroPulse.h"

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
	}
}
