// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveSpawnerActor.generated.h"

UCLASS()
class FIRSTGAMECPP_API AWaveSpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveSpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnWave();

private:
	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 currentValue;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<int32> Waves;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> SpawnedWavePositions;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	USoundBase* WaveSound;

	UPROPERTY(VisibleAnywhere, Category = "Spawn")
	TArray<USceneComponent*> SpawnComponents;


public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> Enemy;

	/*void SpawnEnemies();*/
	UFUNCTION()
	void CheckForEnemy();

private:
	FTimerHandle CheckEnemiesTimerHandle;

public:
	UFUNCTION()
	void HandleNextWave();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText MyText;

};
