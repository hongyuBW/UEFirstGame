// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnMesh.h"
#include "Components/BoxComponent.h"
#include "A_PrimitiveFountain.generated.h"

UCLASS()
class PRIMITIVEFOUNTAIN_API AA_PrimitiveFountain : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AA_PrimitiveFountain();

	UPROPERTY(EditDefaultsOnly, Category = Collision)
	UBoxComponent* BoxCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fountain)
	UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fountain)
	UMaterial* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fountain)
	FVector Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fountain)
	float Lifetime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fountain)
	float Interval = .01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fountain)
	float VerticalSpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fountain)
	float HorizontalSpeed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fountain)
	int SpawnOffset = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fountain)
	bool Spawn = true;

	void SpawnMesh();
	UFUNCTION(BlueprintCallable, Category = Fountain)
	void SetSpawn(bool Condition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	float CurrentTime = Interval;
};
