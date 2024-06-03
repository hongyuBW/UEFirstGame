// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnMesh.generated.h"

UCLASS()
class PRIMITIVEFOUNTAIN_API ASpawnMesh : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	float Lifetime;
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	float VerticalSpeed;
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	float HorizontalSpeed;
	
public:	
	// Sets default values for this actor's properties
	ASpawnMesh();

	void Init(UStaticMesh* _StaticMesh, UMaterial* _Material, FVector _Scale, float _Lifetime, float _VerticalSpeed, float _HorizontalSpeed);

	void Move();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
