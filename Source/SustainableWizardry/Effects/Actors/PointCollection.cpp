// Fill out your copyright notice in the Description page of Project Settings.


#include "PointCollection.h"

// Sets default values
APointCollection::APointCollection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APointCollection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

