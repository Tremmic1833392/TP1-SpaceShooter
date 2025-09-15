// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NiagaraFunctionLibrary.h"
#include "Meteor.generated.h"

UCLASS()
class TPSPACESHIP_API AMeteor : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMeteor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void CheckHealth();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(Blueprintable,EditAnywhere,Category="Meteor")
	float MeteorSpeed = 800;

	UPROPERTY(Blueprintable,BlueprintReadOnly,Category="Meteor")
	float health;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* MeteorExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* MeteorSound; 

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void setHealth();
	void SelfDestroy();
	
	UFUNCTION()
	void onHitMeteor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					bool bFromSweep, const FHitResult& SweepResult);

};
