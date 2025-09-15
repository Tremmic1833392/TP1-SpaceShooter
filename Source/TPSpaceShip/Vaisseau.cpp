// Fill out your copyright notice in the Description page of Project Settings.


#include "Vaisseau.h"
#include "Engine/LocalPlayer.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MaterialHLSLTree.h"
#include "TPSpaceShipCharacter.h"
#include "EngineUtils.h"
#include "Meteor.h"
#include "Camera/CameraActor.h"
#include "Projectile.h"
#include "Components/BoxComponent.h"


// Sets default values
AVaisseau::AVaisseau()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetWorldScale3D(FVector(4.18f, 2.46f, 0.78f));
	RootComponent = BoxComponent;
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	BoxComponent->SetEnableGravity(false);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetCollisionProfileName(TEXT("BlockAll"));
	
	

}

// Called when the game starts or when spawned
void AVaisseau::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(IMC_Vaisseau, 0);
			}
		}
	}

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVaisseau::OnHit);

}

// Called every frame
void AVaisseau::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector Velocity = GetVelocity();
	const float Speed = Velocity.Size();

	bIsMoving = Speed > 5.0f;

}

// Called to bind functionality to input
void AVaisseau::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveForwardAction)
		{
			EnhancedInputComponent->BindAction(ShootAction,ETriggerEvent::Triggered,this,&AVaisseau::Shoot);
			EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AVaisseau::MoveForward);
			//EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Completed, this, &AVaisseau::StopMovement);

			EnhancedInputComponent->BindAction(MoveBehindAction, ETriggerEvent::Triggered, this, &AVaisseau::MoveBehind);
			//EnhancedInputComponent->BindAction(MoveBehindAction, ETriggerEvent::Completed, this, &AVaisseau::StopMovement);

			EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &AVaisseau::MoveLeft);
			//EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &AVaisseau::StopMovement);

			EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AVaisseau::MoveRight);
			//EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &AVaisseau::StopMovement);
			
		}
	}
	
}

void AVaisseau::MoveForward(const FInputActionValue& Value)
{
	
	float AxisValue = Value.Get<float>();

	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		FVector Direction = GetActorForwardVector();
		AddActorLocalOffset(Direction * AxisValue * VaisseauSpeedFB * GetWorld()->DeltaTimeSeconds, true);
		bIsMoving= true;
	}
	
}

void AVaisseau::MoveBehind(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		FVector Direction = GetActorForwardVector() * -1;
		AddActorLocalOffset(Direction * AxisValue * VaisseauSpeedFB * GetWorld()->DeltaTimeSeconds,true);
		bIsMoving= true;
	}
	
}

void AVaisseau::MoveLeft(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		FVector Direction = GetActorRightVector() * -1;
		AddActorLocalOffset(Direction * AxisValue * VaisseauSpeedLR * GetWorld()->DeltaTimeSeconds,true);
		bIsMoving = true;
	}
}
void AVaisseau::MoveRight(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		FVector Direction = GetActorRightVector();
		AddActorLocalOffset(Direction * AxisValue * VaisseauSpeedLR * GetWorld()->DeltaTimeSeconds,true);
		bIsMoving= true;
	}
}
void AVaisseau::Shoot(const FInputActionValue& Value)
{
	if (!canShoot) return;
	
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 250.0f;
	FVector SpawnLocation2 = GetActorLocation() + GetActorForwardVector() * 200.0f;
	FRotator Rotator = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(SpawnLocation,Rotator,SpawnParams);
	FTimerHandle TimerHandle;
	canShoot = false;
	GetWorldTimerManager().SetTimer(TimerHandle,this,&AVaisseau::CanShoot,FireRate,false);
	if (MuzzleFlash)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),MuzzleFlash,SpawnLocation2,Rotator);
	}
}

void AVaisseau::CanShoot()
{
	canShoot = true;
}

void AVaisseau::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					bool bFromSweep, const FHitResult& SweepResult)
{

	
	if (OtherActor != this && OtherActor != nullptr) // Pas de hit a lui meme 
	{
		AMeteor* Meteor = Cast<AMeteor>(OtherActor);
		if (Meteor)
		{
			HealthVaisseau -= 1;
			Meteor->Destroy();
		}
	}
}
void AVaisseau::StopMovement()
{
	bIsMoving = false;
}




	


