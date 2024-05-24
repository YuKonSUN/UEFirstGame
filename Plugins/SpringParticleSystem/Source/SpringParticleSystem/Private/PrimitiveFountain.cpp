// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveFountain.h"

// Sets default values
APrimitiveFountain::APrimitiveFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PrimaryActorTick.bCanEverTick = true;

    MinSpawnInterval = 0.1f;
    MaxSpawnInterval = 0.1f;
    SpawnVelocity = 100.0f;
    SpawnArea = FVector(0.0f,0.0f, 0.0f);
    MinScale = FVector(0.1f);
    MaxScale = FVector(0.2f);
    LaunchAngle = 60.f;
    LifeTime = 5.f;

    ParticlePoolSize = 200;
    CurrentIndex = 0;
    TimeUntilNextSpawn = FMath::FRandRange(MinSpawnInterval, MaxSpawnInterval);

}

// Called when the game starts or when spawned
void APrimitiveFountain::BeginPlay()
{
	Super::BeginPlay();
	
    for (int i = 0; i < ParticlePoolSize; i++)
    {
        FParticle particle;
        // Choose a random mesh and material
        UStaticMesh* ChosenMesh = MeshTypes[FMath::RandRange(0, MeshTypes.Num() - 1)];
        //UMaterial* ChosenMaterial = Materials[FMath::RandRange(0, Materials.Num() - 1)];

        // Create the static mesh component
        particle.MeshComponent = NewObject<UStaticMeshComponent>(this);
        particle.MeshComponent->SetStaticMesh(ChosenMesh);
        //MeshComponent->SetMaterial(0, ChosenMaterial);
        particle.MeshComponent->RegisterComponentWithWorld(GetWorld());

        // Set a random location within the spawn area
        particle.InitialLocation = GetActorLocation() + FVector(
            FMath::FRandRange(-SpawnArea.X / 2.0f, SpawnArea.X / 2.0f),
            FMath::FRandRange(-SpawnArea.Y / 2.0f, SpawnArea.Y / 2.0f),
            SpawnArea.Z
        );
        particle.Location = particle.InitialLocation;

        particle.MeshComponent->SetWorldLocation(particle.InitialLocation);

        // Set a random scale
        particle.Size = FVector(
            FMath::FRandRange(MinScale.X, MaxScale.X),
            FMath::FRandRange(MinScale.Y, MaxScale.Y),
            FMath::FRandRange(MinScale.Z, MaxScale.Z)
        );

        particle.MeshComponent->SetWorldScale3D(particle.Size);

        float LaunchThetaRadians = FMath::DegreesToRadians(FMath::FRandRange(-180.f, 180.f));
        float LaunchPhiRadians = FMath::DegreesToRadians(FMath::FRandRange(-LaunchAngle / 2.f, LaunchAngle / 2.f));
        // Set a random velocity
        particle.InitialVelocity = FVector(
            FMath::Sin(LaunchThetaRadians) * FMath::Sin(LaunchPhiRadians) * SpawnVelocity,
            FMath::Cos(LaunchThetaRadians) * FMath::Sin(LaunchPhiRadians) * SpawnVelocity,
            FMath::Cos(LaunchPhiRadians) * SpawnVelocity
        );

        particle.MeshComponent->SetSimulatePhysics(false);
        particle.MeshComponent->SetEnableGravity(false);
        //particle.MeshComponent->SetPhysicsLinearVelocity(FVector(0));
        //particle.MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        /*particle.MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        particle.MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
        particle.MeshComponent->BodyInstance.SetCollisionProfileName("PhysicsActor");*/


        particle.MeshComponent->SetVisibility(false, false);
        particle.MeshComponent->SetHiddenInGame(true, true);
        particle.bIsActive = false;

        ParticlePool.Add(particle);
    }
    

}

// Called every frame
void APrimitiveFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    for (auto& particle : ParticlePool)
    {
        if (!particle.bIsActive)
        {
            continue;
        }

        particle.Location += particle.InitialVelocity * DeltaTime;
        particle.MeshComponent->SetWorldLocation(particle.Location);

        if (particle.LifeTime <= 0)
        {
            particle.bIsActive = false;
            particle.MeshComponent->SetVisibility(false, false);
            particle.MeshComponent->SetHiddenInGame(true, true);
            //particle.MeshComponent->SetPhysicsLinearVelocity(FVector(0));
            particle.Location = particle.InitialLocation;
        }
        
        particle.LifeTime -= DeltaTime;
    }

    TimeUntilNextSpawn -= DeltaTime;

    if (TimeUntilNextSpawn <= 0.0f)
    {
        SpawnParticle();
        TimeUntilNextSpawn = FMath::FRandRange(MinSpawnInterval, MaxSpawnInterval);
    }

}

//void APrimitiveFountain::SpawnPrimitive()
//{
//    //if (MeshTypes.Num() == 0 || Materials.Num() == 0)
//    if (MeshTypes.Num() == 0)
//    {
//        return;
//    }
//
//    UWorld* World = GetWorld();
//    if (!World)
//    {
//        return;
//    }
//
//    
//}

void APrimitiveFountain::SpawnParticle()
{
    FParticle& particle = ParticlePool[CurrentIndex];
    particle.bIsActive = true;
    particle.MeshComponent->SetVisibility(true, true);
    particle.MeshComponent->SetHiddenInGame(false, false);
    particle.MeshComponent->SetWorldLocation(particle.InitialLocation);
    //particle.MeshComponent->SetPhysicsLinearVelocity(particle.InitialVelocity);
    particle.LifeTime = LifeTime;

    CurrentIndex = (CurrentIndex + 1) % ParticlePoolSize;

}

