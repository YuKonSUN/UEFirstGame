// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveFountain.h"

// Sets default values
APrimitiveFountain::APrimitiveFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PrimaryActorTick.bCanEverTick = true;

    MinSpawnInterval = 0.5f;
    MaxSpawnInterval = 2.0f;
    MinSpawnVelocity = 200.0f;
    MaxSpawnVelocity = 600.0f;
    SpawnArea = FVector(200.0f, 200.0f, 50.0f);
    MinScale = FVector(0.5f);
    MaxScale = FVector(2.0f);

    TimeUntilNextSpawn = FMath::FRandRange(MinSpawnInterval, MaxSpawnInterval);

}

// Called when the game starts or when spawned
void APrimitiveFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APrimitiveFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    TimeUntilNextSpawn -= DeltaTime;

    if (TimeUntilNextSpawn <= 0.0f)
    {
        SpawnPrimitive();
        TimeUntilNextSpawn = FMath::FRandRange(MinSpawnInterval, MaxSpawnInterval);
    }
}

void APrimitiveFountain::SpawnPrimitive()
{
    //if (MeshTypes.Num() == 0 || Materials.Num() == 0)
    if (MeshTypes.Num() == 0)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // Choose a random mesh and material
    UStaticMesh* ChosenMesh = MeshTypes[FMath::RandRange(0, MeshTypes.Num() - 1)];
    //UMaterial* ChosenMaterial = Materials[FMath::RandRange(0, Materials.Num() - 1)];

    // Create the static mesh component
    UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this);
    MeshComponent->SetStaticMesh(ChosenMesh);
    //MeshComponent->SetMaterial(0, ChosenMaterial);
    MeshComponent->RegisterComponentWithWorld(World);

    // Set a random location within the spawn area
    FVector Location = GetActorLocation() + FVector(
        FMath::FRandRange(-SpawnArea.X / 2.0f, SpawnArea.X / 2.0f),
        FMath::FRandRange(-SpawnArea.Y / 2.0f, SpawnArea.Y / 2.0f),
        SpawnArea.Z
    );

    MeshComponent->SetWorldLocation(Location);

    // Set a random scale
    FVector Scale = FVector(
        FMath::FRandRange(MinScale.X, MaxScale.X),
        FMath::FRandRange(MinScale.Y, MaxScale.Y),
        FMath::FRandRange(MinScale.Z, MaxScale.Z)
    );

    MeshComponent->SetWorldScale3D(Scale);

    // Set a random velocity
    FVector Velocity = FVector(
        FMath::FRandRange(-MaxSpawnVelocity, MaxSpawnVelocity),
        FMath::FRandRange(-MaxSpawnVelocity, MaxSpawnVelocity),
        FMath::FRandRange(MinSpawnVelocity, MaxSpawnVelocity)
    );

    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->SetEnableGravity(false);
    MeshComponent->SetPhysicsLinearVelocity(Velocity);
}

