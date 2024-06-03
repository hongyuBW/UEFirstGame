// Fill out your copyright notice in the Description page of Project Settings.


#include "A_PrimitiveFountain.h"

// Sets default values
AA_PrimitiveFountain::AA_PrimitiveFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}


void AA_PrimitiveFountain::SpawnMesh()
{
	FVector SpawnLocation;
	SpawnLocation.X += GetActorLocation().X + FMath::RandRange(-SpawnOffset, SpawnOffset);
	SpawnLocation.Y += GetActorLocation().Y + FMath::RandRange(-SpawnOffset, SpawnOffset);
	SpawnLocation.Z += GetActorLocation().Z + FMath::RandRange(0, SpawnOffset);
	ASpawnMesh* SM = Cast<ASpawnMesh>(GetWorld()->SpawnActor(ASpawnMesh::StaticClass(), &SpawnLocation));
	SM->Init(Mesh, Material, Scale, Lifetime, VerticalSpeed, HorizontalSpeed);
	SM->Move();
}

void AA_PrimitiveFountain::SetSpawn(bool Condition)
{
	Spawn = Condition;
}

// Called when the game starts or when spawned
void AA_PrimitiveFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_PrimitiveFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Spawn)
	{
		CurrentTime -= DeltaTime;
		if (CurrentTime <= .0f)
		{
			SpawnMesh();
			CurrentTime = Interval;
		}
	}
}

