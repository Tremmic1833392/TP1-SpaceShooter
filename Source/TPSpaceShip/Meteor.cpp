// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteor.h"

#include "Projectile.h"

#include "Vaisseau.h"
#include "Components/SphereComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AMeteor::AMeteor()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	SphereComponent->SetEnableGravity(false);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SphereComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	StaticMesh->SetStaticMesh(SphereMesh.Object);
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bAutoActivate = true;
	ProjectileMovement->InitialSpeed = MeteorSpeed;
	ProjectileMovement->MaxSpeed = MeteorSpeed;
	setHealth();
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Explosion(TEXT("/Game/Basic_VFX/Niagara/NS_Basic_13.NS_Basic_13"));
	MeteorExplosion = Explosion.Object;
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundAsset(TEXT("/Game/SFX/MeteorDeadSound.MeteorDeadSound")); 
	MeteorSound = SoundAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Game/StarterContent/Materials/M_Rock_Basalt.M_Rock_Basalt"));
	if (MatFinder.Succeeded())
	{
		StaticMesh->SetMaterial(0, MatFinder.Object);
	}
}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	Super::BeginPlay();
	
	FVector DirectionVector = (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
	ProjectileMovement->Velocity = DirectionVector* MeteorSpeed; // Envoyer le meteor dans la direction du vecteur trouver plus haut
	FTimerHandle TimerHandleLifeTime;
	float lifeTime = 8.0f;
	GetWorldTimerManager().SetTimer(TimerHandleLifeTime, this, &AMeteor::SelfDestroy, lifeTime, false);
	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeteor::onHitMeteor);
	}
}

// Called every frame
void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMeteor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMeteor::setHealth()
{
	health =  FMath::RandRange(1,5);
}

void AMeteor::SelfDestroy()
{
	Destroy();
}

void AMeteor::CheckHealth()
{
	if (health <= 0)
	{
		AVaisseau* Player = Cast<AVaisseau>(GetWorld()->GetFirstPlayerController()->GetPawn());
		Player->Score = Player->Score +100;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::FromInt(Player->Score));
		FVector SpawnLocation = GetActorLocation();

		FTimerHandle TimerHandleBeforeDead;
		SetActorHiddenInGame(true);
		GetWorldTimerManager().SetTimer(TimerHandleBeforeDead, this, &AMeteor::SelfDestroy, 1, false);
		
		if (MeteorExplosion)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "boom");
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),MeteorExplosion,SpawnLocation);
			FVector Location = GetActorLocation(); 
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), MeteorSound, Location, 1.0f, 1.0f, 0.0f); 
		}
		
	}
}

void AMeteor::onHitMeteor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (Cast<AProjectile>(OtherActor) )
		{
			health -= 1;
			CheckHealth();
			OtherActor->Destroy();
		}
	}
}

