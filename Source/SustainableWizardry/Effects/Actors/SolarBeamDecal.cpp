// Fill out your copyright notice in the Description page of Project Settings.


#include "SolarBeamDecal.h"
#include "Components/DecalComponent.h"

// Sets default values
ASolarBeamDecal::ASolarBeamDecal()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
	
}


void ASolarBeamDecal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASolarBeamDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

