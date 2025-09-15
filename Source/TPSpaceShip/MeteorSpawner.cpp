// Fill out your copyright notice in the Description page of Project Settings.


#include "MeteorSpawner.h"

#include "Meteor.h"


// Sets default values
AMeteorSpawner::AMeteorSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AMeteorSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnTimeCooldown = FMath::RandRange(1,10);
	FTimerHandle ThMeteorSpawner;
	GetWorldTimerManager().SetTimer(ThMeteorSpawner,this,&AMeteorSpawner::SpawnMeteor,SpawnTimeCooldown,true);
	
}

// Called every frame
void AMeteorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMeteorSpawner::SpawnMeteor()
{
	
	SpawnTimeCooldown = FMath::RandRange(1,10);
	FVector SpawnLocation = GetActorLocation();
	FRotator Rotator = GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	AMeteor* SpawnedMeteor = GetWorld()->SpawnActor<AMeteor>(SpawnLocation,Rotator,SpawnParams);
	
}
 
