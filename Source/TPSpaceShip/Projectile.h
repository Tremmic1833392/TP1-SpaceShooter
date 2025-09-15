// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"
UCLASS()
class TPSPACESHIP_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(Blueprintable,EditAnywhere,Category="Projectile")
	float ProjectileSpeed = 7000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* ProjectileSound; 
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
