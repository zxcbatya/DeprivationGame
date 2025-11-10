#include "BaseCharacter.h"
#include "Vehicle/DeprivationCar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/IInteractable.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetMesh(), FName("headSocket"));
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CameraComponent->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
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

void ABaseCharacter::EnterVehicle(ADeprivationCar* Vehicle)
{
	if (!Vehicle || CurrentVehicle)
		return;

	CurrentVehicle = Vehicle;

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->SetMovementMode(MOVE_None);
		MovementComp->StopMovementImmediately();
		MovementComp->SetComponentTickEnabled(false);
	}
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	AttachToComponent(Vehicle->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (AController* MyController = GetController())
	{
		MyController->UnPossess();
		MyController->Possess(Vehicle);
	}

	Vehicle->EnterVehicle(this);
}

void ABaseCharacter::ExitVehicle()
{
	if (!CurrentVehicle)
		return;

	CurrentVehicle->ExitVehicle();
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
	if (HitActor)
	{
		if (ADeprivationCar* Vehicle = Cast<ADeprivationCar>(HitActor))
		{
			float Distance = FVector::Dist(GetActorLocation(), HitActor->GetActorLocation());
			if (Distance <= 500.0f && Vehicle->CanInteract_Implementation(Cast<APawn>(const_cast<ABaseCharacter*>(this))))
			{
				return HitActor;
			}
		}
		else if (HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			float Distance = FVector::Dist(GetActorLocation(), HitActor->GetActorLocation());
			if (IInteractable::Execute_CanInteract(HitActor, Cast<APawn>(const_cast<ABaseCharacter*>(this))) && 
				Distance <= IInteractable::Execute_GetInteractionDistance(HitActor))
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
	if (Interactable)
	{
		if (ADeprivationCar* Vehicle = Cast<ADeprivationCar>(Interactable))
		{
			return Vehicle->GetInteractionText_Implementation();
		}
		else if (Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			return IInteractable::Execute_GetInteractionText(Interactable);
		}
	}
	return FText::GetEmpty();
}

void ABaseCharacter::Interactt()
{
	if (CurrentVehicle)
	{
		ExitVehicle();
		return;
	}

	AActor* Interactable = GetInteractableActor();
	if (Interactable)
	{
		if (ADeprivationCar* Vehicle = Cast<ADeprivationCar>(Interactable))
		{
			Vehicle->OnInteract_Implementation(Cast<APawn>(this));
		}
		else if (Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_OnInteract(Interactable, Cast<APawn>(this));
		}
	}
}
