// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	CapsuleComponent->SetEnableGravity(false);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(CapsuleComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	StaticMesh->SetStaticMesh(SphereMesh.Object);
	StaticMesh->SetWorldScale3D(FVector(1,0.5,1));
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Game/FXVarietyPack/Materials/M_ky_storm01.M_ky_storm01"));
	if (MatFinder.Succeeded())
	{
		StaticMesh->SetMaterial(0, MatFinder.Object);
	}
	
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	static ConstructorHelpers::FObjectFinder<USoundWave> SoundAsset(TEXT("/Game/SFX/mixkit-short-laser-gun-shot-1670.mixkit-short-laser-gun-shot-1670")); 
	ProjectileSound = SoundAsset.Object;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileSpeed;

	FVector Location = GetActorLocation(); 
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSound, Location, 1.0f, 1.0f, 0.0f); 
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

