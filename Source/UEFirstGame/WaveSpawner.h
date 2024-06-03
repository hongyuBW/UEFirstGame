// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveSpawner.generated.h"

UCLASS()
class UEFIRSTGAME_API AWaveSpawner : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = Wave)
	USoundBase* WaveSound;

	UPROPERTY(EditAnywhere, Category = Wave)
	TArray<FTransform> SpawnTransform;

	UPROPERTY(EditAnywhere, Category = Wave)
	TSubclassOf<AActor> Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Wave, meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> WaveMap;

	UPROPERTY(EditAnywhere, Category = Wave)
	TArray<FString> WaveKey;

	UPROPERTY(EditAnywhere, Category = Wave)
	TArray<int32> WaveValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Wave, meta = (AllowPrivateAccess = "true"))
	int32 CurrentWave = 0;

public:	
	// Sets default values for this actor's properties
	AWaveSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckForEnemies();

};
