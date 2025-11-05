#include "Vehicle/DeprivationCar.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h" // Правильный инклюд

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "DeprivationCar"

DEFINE_LOG_CATEGORY(LogDeprivationCar);

ADeprivationCar::ADeprivationCar()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCenterOfMass(FVector(0.f, 0.f, 10.f)); // Lower center of mass for better stability
}

void ADeprivationCar::EnterVehicle(APawn* Pawn)
{
	if (!Pawn || CurrentDriver)
		return;

	// Prevent immediate exit after entry
	bCanExitVehicle = false;

	CurrentDriver = Pawn;

	if (AController* DriverController = CurrentDriver->GetController())
	{
		DriverController->UnPossess();
		DriverController->Possess(this);

		if (APlayerController* PC = Cast<APlayerController>(DriverController))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(VehicleMappingContext, 0);
			}
		}

		CurrentDriver->SetActorHiddenInGame(true);
		CurrentDriver->SetActorEnableCollision(false);
		CurrentDriver->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	// Allow exit after a short delay
	FTimerHandle ExitTimerHandle;
	GetWorldTimerManager().SetTimer(ExitTimerHandle, [this]() {
		bCanExitVehicle = true;
	}, 0.5f, false);
}

void ADeprivationCar::ExitVehicle()
{
	// Check if exit is allowed to prevent immediate exit after entry
	if (!CurrentDriver || !bCanExitVehicle)
		return;

	if (AController* DriverController = GetController())
	{
		if (APlayerController* PC = Cast<APlayerController>(DriverController))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(VehicleMappingContext);
			}
		}

		DriverController->UnPossess();
		DriverController->Possess(CurrentDriver);
	}

	FVector ExitLocation = GetActorLocation() + GetActorForwardVector() * 200.0f + GetActorUpVector() * 100.0f;

	CurrentDriver->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentDriver->SetActorLocation(ExitLocation);
	CurrentDriver->SetActorHiddenInGame(false);
	CurrentDriver->SetActorEnableCollision(true);
	CurrentDriver = nullptr;
}

void ADeprivationCar::BeginPlay()
{
	Super::BeginPlay();

	// Improve vehicle stability by adjusting center of mass
	// The center of mass was already adjusted in constructor
}

void ADeprivationCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &ADeprivationCar::Accelerate);
		EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Completed, this, &ADeprivationCar::StopAccelerate);

		EnhancedInput->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ADeprivationCar::Steer);
		EnhancedInput->BindAction(SteerAction, ETriggerEvent::Completed, this, &ADeprivationCar::StopSteer);

		EnhancedInput->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &ADeprivationCar::HandbrakePressed);
		EnhancedInput->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ADeprivationCar::HandbrakeReleased);
		
		// Adding brake input binding for reverse gear
		EnhancedInput->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ADeprivationCar::Brake);
		EnhancedInput->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ADeprivationCar::StopBrake);

		// Bind exit action
		EnhancedInput->BindAction(ExitAction, ETriggerEvent::Triggered, this, &ADeprivationCar::ExitVehicle);
	}
	
}

void ADeprivationCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeprivationCar::Accelerate(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(
		GetVehicleMovement()))
	{
		float ThrottleValue = Value.Get<float>();
		VehicleMovement->SetThrottleInput(ThrottleValue);
	}
}

void ADeprivationCar::StopAccelerate()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(
		GetVehicleMovement()))
	{
		VehicleMovement->SetThrottleInput(0.0f);
	}
}

void ADeprivationCar::Steer(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(
		GetVehicleMovement()))
	{
		float SteerValue = Value.Get<float>();
		VehicleMovement->SetSteeringInput(SteerValue);
	}
}

void ADeprivationCar::StopSteer()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(
		GetVehicleMovement()))
	{
		VehicleMovement->SetSteeringInput(0.0f);
	}
}

void ADeprivationCar::HandbrakePressed()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(
		GetVehicleMovement()))
	{
		VehicleMovement->SetHandbrakeInput(true);
	}
}

void ADeprivationCar::HandbrakeReleased()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(
		GetVehicleMovement()))
	{
		VehicleMovement->SetHandbrakeInput(false);
	}
}

// Adding brake functionality for reverse gear
void ADeprivationCar::Brake(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(
		GetVehicleMovement()))
	{
		// For reverse gear, we use the brake input to control reverse movement
		float BrakeValue = Value.Get<float>();
		VehicleMovement->SetBrakeInput(BrakeValue);
		
		// When brake is pressed and vehicle is slow or stationary, engage reverse
		// This simulates the "reverse as brake" behavior
		if (FMath::Abs(GetVelocity().Size()) < 10.0f) // Vehicle is nearly stationary
		{
			VehicleMovement->SetThrottleInput(-BrakeValue);
		}
	}
}

void ADeprivationCar::StopBrake()
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(
		GetVehicleMovement()))
	{
		VehicleMovement->SetBrakeInput(0.0f);
		VehicleMovement->SetThrottleInput(0.0f);
	}
}

#undef LOCTEXT_NAMESPACE
