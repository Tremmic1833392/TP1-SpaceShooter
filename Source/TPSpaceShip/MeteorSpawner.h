// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorSpawner.generated.h"

UCLASS()
class TPSPACESHIP_API AMeteorSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeteorSpawner();

	float SpawnTimeCooldown;

	UFUNCTION()
	void SpawnMeteor();
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
