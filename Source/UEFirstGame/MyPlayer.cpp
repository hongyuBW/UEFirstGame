// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogTempPlayer);

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	AttackArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("AttackArrow"));
	AttackArrow->SetupAttachment(RootComponent);

	/*static ConstructorHelpers::FObjectFinder<UUserWidget> HealthBarWidget(TEXT("/Game/UI/WB_HealthBar"));
	static ConstructorHelpers::FObjectFinder<UUserWidget> HUDWidget(TEXT("/Game/UI/WB_HUD"));*/
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();

	FOnMontageEnded AttackMontageEnded;
	//AnimInstance->Montage_SetEndDelegate(AttackMontageEnded, AttackMontage);
	FScriptDelegate NotifyBegin;
	NotifyBegin.BindUFunction(this, "Attack");
	AnimInstance->OnPlayMontageNotifyBegin.Add(NotifyBegin);

	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AMyPlayer::AttackBegin);
	AnimInstance->OnMontageEnded.AddDynamic(this, &AMyPlayer::AttackEnd);

	if (HealthBarWidgetAsset)
	{
		HealthBarWidget = CreateWidget<UUserWidget>(GetWorld(), HealthBarWidgetAsset);
		if (HealthBarWidget)
			HealthBarWidget->AddToViewport();
	}
	if (HUDWidgetAsset)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetAsset);
		if (HUDWidget)
			HUDWidget->AddToViewport();
	}
	
	
	//UGameplayStatics::ApplyDamage()
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTempPlayer, Error, TEXT("Failed to find an Enhanced Input component!"));
		}
	}
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayer::Look);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyPlayer::Attack);
	}

}

void AMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MoveVector.Y);
		AddMovementInput(RightDirection, MoveVector.X);
	}
}

void AMyPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerPitchInput(LookVector.Y);
		AddControllerYawInput(LookVector.X);
	}
}

void AMyPlayer::Attack()
{
	if (CanAttack)
	{
		AnimInstance->Montage_Play(AttackMontage);
		CanAttack = false;
		
	}

}


void AMyPlayer::AttackBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	FVector Start = AttackArrow->GetComponentLocation();
	FVector End = Start + AttackArrow->GetForwardVector() * 200.f;

	float Radius = 30.f;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	
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

void AMyPlayer::AttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage)
	{
		CanAttack = true;
		UE_LOG(LogTempPlayer, Error, TEXT("aaa"));
	}
	
}

float AMyPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= Damage;

	UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation());

	if (CurrentHealth > .0f)
	{
		FVector LaunchVelocity = GetActorForwardVector() * -160.f + GetActorUpVector() * 80.f;
		LaunchCharacter(LaunchVelocity, false, false);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(TEXT("Custom"));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		DisableInput(GetLocalViewingPlayerController());

		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AMyPlayer::DestroyPlayer, 1.f, false);
	}
	return Damage;
}

void AMyPlayer::DestroyPlayer()
{
	Destroy();
}

