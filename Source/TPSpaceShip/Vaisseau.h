// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"   // Nécessaire pour Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Vaisseau.generated.h"

class UInputAction;
class UInputMappingContext;
UCLASS()
class TPSPACESHIP_API AVaisseau : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVaisseau();

protected:
	// Called when the game starts or when spawned
	// Commentaire exemple de conflit 1
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function 
	UFUNCTION(blueprintCallable)
	void MoveForward(const FInputActionValue& Value);
	
	UFUNCTION(blueprintCallable)
	void MoveBehind(const FInputActionValue& Value);

	UFUNCTION(blueprintCallable)
	void MoveLeft(const FInputActionValue& Value);
	
	UFUNCTION(blueprintCallable)
	void MoveRight(const FInputActionValue& Value);

	UFUNCTION(blueprintCallable)
	void Shoot(const FInputActionValue& Value);

	UFUNCTION(blueprintCallable)
	void CanShoot();

	UFUNCTION(blueprintCallable)
	void StopMovement();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					bool bFromSweep, const FHitResult& SweepResult);
	
	//	Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveBehindAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* ShootAction;

	UPROPERTY(BlueprintReadWrite)
	bool bIsMoving = false;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Vaisseau;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Vaisseau")
	class UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* MuzzleFlash;
	

	// Variables
	UPROPERTY(Blueprintable,EditAnywhere,Category="Vaisseau")
	float VaisseauSpeedFB = 100;
	
	UPROPERTY(Blueprintable,EditAnywhere,Category="Vaisseau")
	float VaisseauSpeedLR = 100;

	UPROPERTY(Blueprintable,EditAnywhere,Category="Vaisseau")
	float FireRate = 0.5f;

	UPROPERTY(Blueprintable,BlueprintReadOnly,Category="Vaisseau")
	float HealthVaisseau = 3;

	bool canShoot= true;

	UPROPERTY(Blueprintable,BlueprintReadOnly,Category="Vaisseau")
	float Score = 0;
	
};
