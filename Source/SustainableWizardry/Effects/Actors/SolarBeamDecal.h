// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SolarBeamDecal.generated.h"

UCLASS()
class SUSTAINABLEWIZARDRY_API ASolarBeamDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	ASolarBeamDecal();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:
	virtual void BeginPlay() override;


};
