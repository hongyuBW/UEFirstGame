// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnMesh.h"

// Sets default values
ASpawnMesh::ASpawnMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetSimulatePhysics(true);
}

void ASpawnMesh::Init(UStaticMesh* _StaticMesh, UMaterial* _Material, FVector _Scale, float _Lifetime, float _VerticalSpeed, float _HorizontalSpeed)
{
	this->StaticMesh->SetStaticMesh(_StaticMesh);
	this->StaticMesh->SetMaterial(0, _Material);
	this->StaticMesh->SetWorldScale3D(_Scale);
	this->Lifetime = _Lifetime;
	this->VerticalSpeed = _VerticalSpeed;
	this->HorizontalSpeed = _HorizontalSpeed;
}

void ASpawnMesh::Move()
{
	StaticMesh->AddImpulse(FVector(HorizontalSpeed * FMath::RandRange(-1, 1), 
		HorizontalSpeed * FMath::RandRange(-1, 1), VerticalSpeed * FMath::RandRange(-1, 1)));
}


// Called when the game starts or when spawned
void ASpawnMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Lifetime -= DeltaTime;
	if (Lifetime <= .0f)
		Destroy();
}

