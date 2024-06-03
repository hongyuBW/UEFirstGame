// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "Logging/LogMacros.h"
#include "UObject/ConstructorHelpers.h"
#include "MyPlayer.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTempPlayer, Log, All);

UCLASS()
class UEFIRSTGAME_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, Category = Battle)
	UArrowComponent* AttackArrow;

	UPROPERTY(EditDefaultsOnly, Category = Battle)
	USoundBase* DamageSound;

	UPROPERTY(EditDefaultsOnly, Category = Anim)
	UAnimMontage* AttackMontage;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> HealthBarWidgetAsset;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> HUDWidgetAsset;
	UUserWidget* HealthBarWidget;
	UUserWidget* HUDWidget;

	

public:
	// Sets default values for this character's properties
	AMyPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float CurrentHealth = MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	bool CanAttack = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle DestroyTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack();

	UFUNCTION()
	void AttackBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
	void AttackEnd(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void DestroyPlayer();
};
