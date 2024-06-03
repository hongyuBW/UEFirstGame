// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "MyEnemy.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "MyEnemy.h"

// Sets default values
AWaveSpawner::AWaveSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	WaveMap.Add("Wave 1", 3);
	WaveMap.Add("Wave 2", 5);
	WaveMap.Add("Wave 3", 8);

	WaveKey.Add("Wave 1");
	WaveKey.Add("Wave 2");
	WaveKey.Add("Wave 3");

	WaveValue.Add(3);
	WaveValue.Add(5);
	WaveValue.Add(8);

	CheckForEnemies();
}

// Called every frame
void AWaveSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveSpawner::CheckForEnemies()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyEnemy::StaticClass(), Enemies);

	if (Enemies.Num() == 0)
	{
		UGameplayStatics::PlaySound2D(this, WaveSound);

		/*TArray<int32> Values;
		TMap<int32, int32> Map;
		UBlueprintMapLibrary::Map_Values(Map,Values);*/

		int32 Value = WaveMap.FindRef(WaveKey[CurrentWave]);

		for (int i = 1; i <Value; i++)
		{
			if (Enemy)
			{
				FTransform Transform = SpawnTransform[FMath::RandRange(0, SpawnTransform.Num() - 1)];
				//GetWorld()->SpawnActor(Enemy, &Transform);
				AMyEnemy* L_Enemy = Cast<AMyEnemy>(GetWorld()->SpawnActor(Enemy, &Transform));
				L_Enemy->SpawnDefaultController();
				UE_LOG(LogTemp, Warning, TEXT("Spawn"));
			}
			
		}

		CurrentWave++;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There are enemies in the level£¡"));
	}
}

