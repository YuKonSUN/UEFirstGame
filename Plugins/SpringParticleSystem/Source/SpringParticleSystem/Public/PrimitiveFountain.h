// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "PrimitiveFountain.generated.h"


USTRUCT(BlueprintType)
struct FParticle
{
    GENERATED_BODY()

    UStaticMeshComponent* MeshComponent;

    FVector Size;

    FVector InitialVelocity;

    FVector InitialLocation;

    FVector Location;

    float LifeTime;

    bool bIsActive;
};

UCLASS()
class SPRINGPARTICLESYSTEM_API APrimitiveFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrimitiveFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    TArray<UStaticMesh*> MeshTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    TArray<UMaterial*> Materials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float MinSpawnInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float MaxSpawnInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float SpawnVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector SpawnArea;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector MinScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector MaxScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float LaunchAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float LifeTime;

private:

    //void SpawnPrimitive();

    void SpawnParticle();

    float TimeUntilNextSpawn;

    TArray<FParticle> ParticlePool;
    int ParticlePoolSize;

    int CurrentIndex;
};
