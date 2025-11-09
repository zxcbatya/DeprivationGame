#include "Vehicle/DeprivationCar.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacter.h"

DEFINE_LOG_CATEGORY(LogDeprivationCar);

ADeprivationCar::ADeprivationCar()
{
	PrimaryActorTick.bCanEverTick = false;
	GetMesh()->SetCenterOfMass(FVector(0.f, 0.f, 10.f));
}

void ADeprivationCar::BeginPlay()
{
	Super::BeginPlay();

	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetComponentTickEnabled(true);
	}
}

void ADeprivationCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		BindInputActions(EnhancedInput);
	}
}

void ADeprivationCar::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetComponentTickEnabled(true);
	}
}

void ADeprivationCar::UnPossessed()
{
	Super::UnPossessed();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(VehicleMappingContext);
		}
	}
}

void ADeprivationCar::EnterVehicle(APawn* Pawn)
{
	if (!Pawn || CurrentDriver)
		return;

	bCanExitVehicle = false;
	bIgnoreNextExit = true;
	CurrentDriver = Pawn;

	FTimerHandle ExitIgnoreTimerHandle;
	FTimerHandle CanExitTimerHandle;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		GetWorldTimerManager().SetTimerForNextTick([this, PC]()
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(VehicleMappingContext, 0);
			}

			if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				BindInputActions(EnhancedInput);
			}
		});

		GetWorldTimerManager().SetTimer(ExitIgnoreTimerHandle, [this]()
		{
			bIgnoreNextExit = false;
		}, 0.3f, false);
	}

	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetComponentTickEnabled(true);
	}

	GetWorldTimerManager().SetTimer(CanExitTimerHandle, [this]()
	{
		bCanExitVehicle = true;
	}, 0.5f, false);
}

void ADeprivationCar::ExitVehicle()
{
	if (bIgnoreNextExit || !CurrentDriver || !bCanExitVehicle)
		return;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(VehicleMappingContext);
		}

		PC->UnPossess();
		PC->Possess(CurrentDriver);
	}

	FVector ExitLocation = GetActorLocation() + GetActorForwardVector() * 200.0f + GetActorUpVector() * 100.0f;
	CurrentDriver->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentDriver->SetActorLocation(ExitLocation);
	CurrentDriver->SetActorHiddenInGame(false);

	if (ACharacter* Character = Cast<ACharacter>(CurrentDriver))
	{
		Character->SetActorEnableCollision(true);
		if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
		{
			MovementComp->SetComponentTickEnabled(true);
			MovementComp->SetMovementMode(MOVE_Walking);
		}
	}

	CurrentDriver = nullptr;
}

void ADeprivationCar::BindInputActions(UEnhancedInputComponent* EnhancedInput)
{
	if (AccelerateAction)
	{
		EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &ADeprivationCar::Accelerate);
		EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Completed, this, &ADeprivationCar::StopAccelerate);
	}

	if (SteerAction)
	{
		EnhancedInput->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ADeprivationCar::Steer);
		EnhancedInput->BindAction(SteerAction, ETriggerEvent::Completed, this, &ADeprivationCar::StopSteer);
	}

	if (HandbrakeAction)
	{
		EnhancedInput->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &ADeprivationCar::HandbrakePressed);
		EnhancedInput->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ADeprivationCar::HandbrakeReleased);
	}

	if (BrakeAction)
	{
		EnhancedInput->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ADeprivationCar::Brake);
		EnhancedInput->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ADeprivationCar::StopBrake);
	}

	if (ExitAction)
	{
		EnhancedInput->BindAction(ExitAction, ETriggerEvent::Triggered, this, &ADeprivationCar::ExitVehicle);
	}
}

void ADeprivationCar::Accelerate(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetThrottleInput(Value.Get<float>());
	}
}

void ADeprivationCar::StopAccelerate()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetThrottleInput(0.0f);
	}
}

void ADeprivationCar::Steer(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetSteeringInput(Value.Get<float>());
	}
}

void ADeprivationCar::StopSteer()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetSteeringInput(0.0f);
	}
}

void ADeprivationCar::HandbrakePressed()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetHandbrakeInput(true);
	}
}

void ADeprivationCar::HandbrakeReleased()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetHandbrakeInput(false);
	}
}

void ADeprivationCar::Brake(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetBrakeInput(Value.Get<float>());
	}
}

void ADeprivationCar::StopBrake()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetBrakeInput(0.0f);
	}
}

bool ADeprivationCar::CanInteract_Implementation(APawn* InteractingPawn) const
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn);
	return Character != nullptr && CurrentDriver == nullptr;
}

void ADeprivationCar::OnInteract_Implementation(APawn* InteractingPawn)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
	{
		if (CanInteract_Implementation(InteractingPawn))
		{
			Character->EnterVehicle(this);
		}
	}
}

FText ADeprivationCar::GetInteractionText_Implementation() const
{
	return FText::FromString(TEXT("Enter Vehicle"));
}

float ADeprivationCar::GetInteractionDistance_Implementation() const
{
	return 500.0f;
}
