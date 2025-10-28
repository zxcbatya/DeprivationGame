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
	GetMesh()->SetCenterOfMass(FVector(0.f, 0.f, 20.f));
}

void ADeprivationCar::EnterVehicle(APawn* Pawn)
{
	if (!Pawn || CurrentDriver)
		return;

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
}

void ADeprivationCar::ExitVehicle()
{
	if (!CurrentDriver)
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

#undef LOCTEXT_NAMESPACE
