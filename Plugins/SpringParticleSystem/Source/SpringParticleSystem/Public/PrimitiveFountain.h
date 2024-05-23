// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrimitiveFountain.generated.h"

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
    float MinSpawnVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    float MaxSpawnVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector SpawnArea;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector MinScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fountain Settings")
    FVector MaxScale;

private:

    void SpawnPrimitive();

    float TimeUntilNextSpawn;
};
