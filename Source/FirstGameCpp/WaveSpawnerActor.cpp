// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawnerActor.h"
#include "XP_EnemyCharacter.h"

// Sets default values
AWaveSpawnerActor::AWaveSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	currentValue = 0;


}

// Called when the game starts or when spawned
void AWaveSpawnerActor::BeginPlay()
{
	Super::BeginPlay();


	Waves.Add(2);
	Waves.Add(4);
	Waves.Add(6);
	SpawnWave();
}

// Called every frame
void AWaveSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveSpawnerActor::SpawnWave()
{
	for (int i = 0; i < Waves[currentValue]; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnedWavePositions.Num() - 1);
		FTransform SpawnTransform = SpawnedWavePositions[RandomIndex];
		
		//FTransform SpawnTransform = GetActorTransform();

		if (AActor* SpawnedActor = GetWorld()->SpawnActor(Enemy, &SpawnTransform))
		{
			AXP_EnemyCharacter* Tmp_Enemy = Cast<AXP_EnemyCharacter>(SpawnedActor);
			Tmp_Enemy->SpawnDefaultController();
			UE_LOG(LogTemp, Log, TEXT("Successfully spawned enemy"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn enemy"));
		}
	}
}

