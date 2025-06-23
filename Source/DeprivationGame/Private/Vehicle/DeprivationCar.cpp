#include "Vehicle/DeprivationCar.h"
#include "ChaosVehicleMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"

ADeprivationCar::ADeprivationCar()
{
	PrimaryActorTick.bCanEverTick = true;

	EnterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterTrigger"));
	EnterTrigger->SetupAttachment(GetRootComponent());
	EnterTrigger->SetBoxExtent(FVector(200.f, 150.f, 100.f));
	EnterTrigger->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	EnterTrigger->SetCollisionProfileName(TEXT("Trigger"));
}

void ADeprivationCar::EnterVehicle(APawn* Pawn)
{
	if (!Pawn || CurrentDriver) return; // Проверка на долбоёба я долбаеб 1 2 3

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
	}

	CurrentDriver->SetActorHiddenInGame(true);
	CurrentDriver->SetActorEnableCollision(false);

	CurrentDriver->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ADeprivationCar::ExitVehicle()
{
	if (!CurrentDriver) return;

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

	FVector ExitLocation = GetActorLocation() +
		GetActorForwardVector() * +200.f +
		GetActorUpVector() * 100.f;

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
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Accelerate
		EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &ADeprivationCar::Accelerate);
		EnhancedInput->BindAction(AccelerateAction, ETriggerEvent::Completed, this, &ADeprivationCar::StopAccelerate);

		// Brake
		EnhancedInput->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ADeprivationCar::Brake);
		EnhancedInput->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ADeprivationCar::StopBrake);

		// Steer
		EnhancedInput->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ADeprivationCar::Steer);
		EnhancedInput->BindAction(SteerAction, ETriggerEvent::Completed, this, &ADeprivationCar::StopSteer);
	}
}

void ADeprivationCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//enter system

void ADeprivationCar::StopAccelerate()
{
	if (GetVehicleMovement())
	{
		GetVehicleMovement()->SetThrottleInput(0.0f);
		GetVehicleMovement()->SetBrakeInput(0.0f);
	}
}

void ADeprivationCar::Accelerate(const FInputActionValue& Value)
{
	if (!GetVehicleMovement()) return;

	float ThrottleValue = Value.Get<float>();
	GetVehicleMovement()->SetThrottleInput(ThrottleValue); // ГАЗ В ОТДЕЛЬНУЮ ЖОПУ
	GetVehicleMovement()->SetBrakeInput(0.0f); // СБРАСЫВАЙ ТОРМОЗ ЕБЛАН
}

void ADeprivationCar::Brake(const FInputActionValue& Value)
{
	if (!GetVehicleMovement()) return;

	float BrakeValue = Value.Get<float>();
	GetVehicleMovement()->SetBrakeInput(BrakeValue); // ТОРМОЗ В ОТДЕЛЬНУЮ ЖОПУ
	GetVehicleMovement()->SetThrottleInput(0.0f); // СБРАСЫВАЙ ГАЗ, ДАУН
}

void ADeprivationCar::StopBrake()
{
	if (GetVehicleMovement())
	{
		GetVehicleMovement()->SetBrakeInput(0.0f);
	}
}

void ADeprivationCar::Steer(const FInputActionValue& Value)
{
	if (GetVehicleMovement())
	{
		float SteerValue = Value.Get<float>();
		GetVehicleMovement()->SetSteeringInput(-SteerValue);
	}
}

void ADeprivationCar::StopSteer()
{
	if (GetVehicleMovement())
	{
		GetVehicleMovement()->SetSteeringInput(0.0f);
	}
}

void ADeprivationCar::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
