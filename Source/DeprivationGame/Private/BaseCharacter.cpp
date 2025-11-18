#include "BaseCharacter.h"
#include "Vehicle/DeprivationCar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/IInteractable.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::SetFatigueState(EFatigueState NewState)
{
	if (TiredState == NewState) return; 
    
	TiredState = NewState;
    
	OnFatigueStateChanged(TiredState);
}

void ABaseCharacter::SetDrunkState(EDrunkState NewState)
{
	if (DrunkState == NewState) return;
    
	DrunkState = NewState;
    
	OnDrunkStateChanged(DrunkState);
}

void ABaseCharacter::OnFatigueStateChanged(EFatigueState State)
{
	// Здесь можно добавить логику для изменения состояния усталости
	// Например, изменение материалов, звуков и т.д.
}

void ABaseCharacter::OnDrunkStateChanged(EDrunkState State)
{
	// Здесь можно добавить логику для изменения состояния опьянения
	// Например, изменение материалов, звуков и т.д.
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::EnterVehicle(APawn* Vehicle)
{
	ADeprivationCar* Car = Cast<ADeprivationCar>(Vehicle);
	if (!Car || CurrentVehicle)
		return;

	CurrentVehicle = Car;

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->SetMovementMode(MOVE_None);
		MovementComp->StopMovementImmediately();
		MovementComp->SetComponentTickEnabled(false);
	}
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	AttachToComponent(Car->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (AController* MyController = GetController())
	{
		MyController->UnPossess();
		MyController->Possess(Vehicle);
	}

	Car->EnterVehicle(Cast<APawn>(this));
}

void ABaseCharacter::ExitVehicle()
{
	if (!CurrentVehicle)
		return;

	if (ADeprivationCar* Car = Cast<ADeprivationCar>(CurrentVehicle))
	{
		Car->ExitVehicle();
	}
	CurrentVehicle = nullptr;
}

AActor* ABaseCharacter::LineTrace(float LineLength, bool bDrawDebug) const
{
	FVector StartLocation;
	FVector ForwardVector;

	if (CameraComponent && CameraComponent->IsValidLowLevel())
	{
		StartLocation = CameraComponent->GetComponentLocation();
		ForwardVector = CameraComponent->GetForwardVector();
	}
	else
	{
		StartLocation = GetActorLocation();
		ForwardVector = GetActorForwardVector();
	}

	FVector EndLocation = StartLocation + ForwardVector * LineLength;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams
	);

	if (bDrawDebug)
	{
		FLinearColor TraceColor = bHit ? FLinearColor::Red : FLinearColor::Green;
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, TraceColor.ToFColor(true), false, 5.0f, 0, 2.0f);
		
		if (bHit)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, TraceColor.ToFColor(true), false, 5.0f);
		}
	}

	return bHit ? HitResult.GetActor() : nullptr;
}

AActor* ABaseCharacter::GetInteractableActor() const
{
	if (CurrentVehicle)
	{
		return CurrentVehicle;
	}

	AActor* HitActor = LineTrace(300.0f, false);
	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		if (IInteractable* Interactable = Cast<IInteractable>(HitActor))
		{
			float Distance = FVector::Dist(GetActorLocation(), HitActor->GetActorLocation());
			if (Interactable->CanInteract(const_cast<ABaseCharacter*>(this)) && 
				Distance <= Interactable->GetInteractionDistance())
			{
				return HitActor;
			}
		}
	}
	return nullptr;
}

FText ABaseCharacter::GetInteractionPrompt() const
{
	AActor* Interactable = GetInteractableActor();
	if (Interactable && Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		if (IInteractable* InteractableInterface = Cast<IInteractable>(Interactable))
		{
			return InteractableInterface->GetInteractionText();
		}
	}
	return FText::GetEmpty();
}

void ABaseCharacter::Interact()
{
	if (CurrentVehicle)
	{
		ExitVehicle();
		return;
	}

	AActor* Interactable = GetInteractableActor();
	if (Interactable && Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		if (IInteractable* InteractableInterface = Cast<IInteractable>(Interactable))
		{
			InteractableInterface->OnInteract(this);
		}
	}
}