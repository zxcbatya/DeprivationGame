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

void ADeprivationCar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ExitAction)
		{
			EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Started, this, &ADeprivationCar::ExitVehicle);
		}
		
		// Привязка действий для управления автомобилем
		if (AccelerateAction)
		{
			EnhancedInputComponent->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &ADeprivationCar::OnAccelerate);
		}
		
		if (BrakeAction)
		{
			EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ADeprivationCar::OnBrake);
		}
		
		if (SteerAction)
		{
			EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ADeprivationCar::OnSteer);
		}
		
		if (HandbrakeAction)
		{
			EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &ADeprivationCar::OnHandbrakePressed);
			EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ADeprivationCar::OnHandbrakeReleased);
		}
	}
}

void ADeprivationCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Функции управления автомобилем
void ADeprivationCar::OnAccelerate(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetThrottleInput(Value.Get<float>());
	}
}

void ADeprivationCar::OnBrake(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetBrakeInput(Value.Get<float>());
	}
}

void ADeprivationCar::OnSteer(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetSteeringInput(Value.Get<float>());
	}
}

void ADeprivationCar::OnHandbrakePressed(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetHandbrakeInput(true);
	}
}

void ADeprivationCar::OnHandbrakeReleased(const FInputActionValue& Value)
{
	if (UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		VehicleMovement->SetHandbrakeInput(false);
	}
}

#undef LOCTEXT_NAMESPACE