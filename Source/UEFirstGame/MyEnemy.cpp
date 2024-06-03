// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/MetaNavMeshPath.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WaveSpawner.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("AttackArrow"));
	AttackArrow->SetupAttachment(RootComponent);

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(!Meshes.IsEmpty())
		GetMesh()->SetSkeletalMesh(Meshes[FMath::RandRange(0, Meshes.Num() - 1)]);
	
	AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		WonderOrChase();
		AIController->ReceiveMoveCompleted.AddDynamic(this, &AMyEnemy::OnMoveCompleted);
	}

	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AMyEnemy::AttackBegin);
	AnimInstance->OnMontageEnded.AddDynamic(this, &AMyEnemy::AttackEnd);

	GetWorld()->GetTimerManager().SetTimer(CreateAIControllerTimerHandle, this, &AMyEnemy::CreateAIController, 1.f, false);
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyEnemy::WonderOrChase()
{
	if (AIController)
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		if (GetDistanceTo(PlayerCharacter) < 500.f)
		{
			AIController->MoveToActor(PlayerCharacter, 5.f, true, true, false, 0, true);
			//UE_LOG(LogTemp, Warning, TEXT("000"));
		}
		else
		{
			//UNavigationSystemV1::GetRandomReachablePointInRadius;
			//FNavLocation NavLocation;
			//UNavigationSystemV1::GetRandomReachablePointInRadius(GetActorLocation(), 500.f, NavLocation, 0);

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, GetActorLocation(), RandomLocation, 500.f);
			AIController->MoveToLocation(RandomLocation, 5.f, true, true, false, false, 0, true);
			//UE_LOG(LogTemp, Warning, TEXT("500"));
		}
		
	}
}

void AMyEnemy::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Attack();
}

void AMyEnemy::Attack()
{
	if (AnimInstance)
		AnimInstance->Montage_Play(AttackMontage);
}

void AMyEnemy::AttackBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	FVector Start = AttackArrow->GetComponentLocation();
	FVector End = Start + AttackArrow->GetForwardVector() * 200.f;

	float Radius = 30.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FHitResult OutHit;

	UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, Radius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

	if (OutHit.bBlockingHit)
	{
		if (OutHit.GetActor())
		{
			UGameplayStatics::ApplyDamage(OutHit.GetActor(), 10.f, GetController(), this, nullptr);
		}

	}
}

void AMyEnemy::AttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage)
	{
		WonderOrChase();
	}
}

float AMyEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= Damage;

	UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation());

	if (Health > .0f)
	{
		FVector LaunchVelocity = GetActorForwardVector() * -200.f + GetActorUpVector() * 100.f;
		LaunchCharacter(LaunchVelocity, false, false);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(TEXT("Custom"));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCharacterMovement()->DisableMovement();

		GetWorld()->GetTimerManager().SetTimer(DestoryTimerHandle, this, &AMyEnemy::DestroyEnemy, 3.f, false);
	}
	return Damage;
}

void AMyEnemy::DestroyEnemy()
{
	Destroy();
	AWaveSpawner* WaveSpawner = Cast<AWaveSpawner>(UGameplayStatics::GetActorOfClass(this, AWaveSpawner::StaticClass()));
	WaveSpawner->CheckForEnemies();
}

void AMyEnemy::CreateAIController()
{
	AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		WonderOrChase();
		AIController->ReceiveMoveCompleted.AddDynamic(this, &AMyEnemy::OnMoveCompleted);
	}
	GetWorld()->GetTimerManager().ClearTimer(CreateAIControllerTimerHandle);
}

