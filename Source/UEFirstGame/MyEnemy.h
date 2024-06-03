// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "MyEnemy.generated.h"

class AAIController;
class UPathFollowingComponent;

UCLASS()
class UEFIRSTGAME_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Battle)
	UArrowComponent* AttackArrow;

	UPROPERTY(EditDefaultsOnly, Category = Battle)
	USoundBase* DamageSound;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	TArray<USkeletalMesh*> Meshes;

	UPROPERTY(EditDefaultsOnly, Category = Anim)
	UAnimMontage* AttackMontage;

	UAnimInstance* AnimInstance;
	
	AAIController* AIController;
	

public:
	// Sets default values for this character's properties
	AMyEnemy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = State)
	float Health = 20.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle DestoryTimerHandle;
	FTimerHandle CreateAIControllerTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void WonderOrChase();

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);


	void Attack();

	UFUNCTION()
	void AttackBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
	void AttackEnd(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void DestroyEnemy();

	void CreateAIController();
};
