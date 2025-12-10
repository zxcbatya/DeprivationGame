#include "BaseCharacter.h"
#include "Vehicle/DeprivationCar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/IInteractable.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Interfaces/IWidgetAnimationHandler.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraOffset = FVector(0.0f, 70.0f, 170.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetMesh());
	CameraComponent->SetRelativeLocation(CameraOffset);
	CameraComponent->bUsePawnControlRotation = true;

	InteractionCheckTimer = 0.0f;
	CurrentHoveredInteractable = nullptr;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateCrosshairWidget();
	CreateInteractionWidget();

	if (CreatedCrosshairWidget)
	{
		CreatedCrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
		ShowCrosshair(false);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraComponent->SetRelativeLocation(CameraOffset);

	InteractionCheckTimer += DeltaTime;
	if (InteractionCheckTimer >= 0.1f)
	{
		InteractionCheckTimer = 0.1f;

		AActor* Interactable = GetInteractableActor();

		if (Interactable != CurrentHoveredInteractable)
		{
			CurrentHoveredInteractable = Interactable;

			if (Interactable != nullptr)
			{
				if (CreatedCrosshairWidget && CreatedCrosshairWidget->GetClass()->ImplementsInterface(
					UWidgetAnimationHandler::StaticClass()))
				{
					IWidgetAnimationHandler::Execute_PlayShowAnimation(CreatedCrosshairWidget);
					CreatedCrosshairWidget->SetVisibility(ESlateVisibility::Visible);
					ShowCrosshair(true);
				}

				if (CreatedInteractionWidget && CreatedInteractionWidget->GetClass()->ImplementsInterface(
					UWidgetAnimationHandler::StaticClass()))
				{
					IWidgetAnimationHandler::Execute_PlayShowAnimation(CreatedInteractionWidget);
				}
			}
			else
			{
				if (CreatedCrosshairWidget && CreatedCrosshairWidget->GetClass()->ImplementsInterface(
					UWidgetAnimationHandler::StaticClass()))
				{
					IWidgetAnimationHandler::Execute_PlayHideAnimation(CreatedCrosshairWidget);
				}

				if (CreatedInteractionWidget && CreatedInteractionWidget->GetClass()->ImplementsInterface(
					UWidgetAnimationHandler::StaticClass()))
				{
					IWidgetAnimationHandler::Execute_PlayHideAnimation(CreatedInteractionWidget);
				}
			}
		}

		bool bIsHovering = Interactable != nullptr;
		FText Prompt = bIsHovering ? GetInteractionPrompt() : FText::GetEmpty();
		ShowInteractionPrompt(bIsHovering, Prompt);
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind interaction input
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind the interaction action if it's set
		if (InteractionAction)
		{
			EnhancedInput->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Interact);
		}
		
		// Add mapping context if it's set
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				if (CharacterMappingContext)
				{
					Subsystem->AddMappingContext(CharacterMappingContext, 0);
				}
			}
		}
	}
}

void ABaseCharacter::EnterVehicle(APawn* Vehicle)
{
	ADeprivationCar* Car = Cast<ADeprivationCar>(Vehicle);
	if (!Car || CurrentVehicle) return;

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
	if (!CurrentVehicle) return;

	if (ADeprivationCar* Car = Cast<ADeprivationCar>(CurrentVehicle))
	{
		Car->ExitVehicle();
	}

	CurrentVehicle = nullptr;
}

AActor* ABaseCharacter::LineTrace(float LineLength, bool bDrawDebug) const
{
	FVector Start = CameraComponent ? CameraComponent->GetComponentLocation() : GetActorLocation();
	FVector End = Start + (CameraComponent ? CameraComponent->GetForwardVector() : GetActorForwardVector()) *
		LineLength;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (bDrawDebug)
	{
		FColor Color = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), Start, End, Color, false, 2.0f, 0, 2.0f);
		if (bHit) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, Color, false, 2.0f);
	}

	return bHit ? Hit.GetActor() : nullptr;
}

AActor* ABaseCharacter::GetInteractableActor() const
{
	if (CurrentVehicle) return CurrentVehicle;

	AActor* HitActor = LineTrace(300.0f, false);
	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		float Dist = FVector::Dist(GetActorLocation(), HitActor->GetActorLocation());
		if (IInteractable::Execute_CanInteract(HitActor, const_cast<ABaseCharacter*>(this)) &&
			Dist <= IInteractable::Execute_GetInteractionDistance(HitActor))
		{
			return HitActor;
		}
	}
	return nullptr;
}

FText ABaseCharacter::GetInteractionPrompt() const
{
	AActor* Interactable = GetInteractableActor();
	if (Interactable && Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return IInteractable::Execute_GetInteractionText(Interactable);
	}
	return FText::GetEmpty();
}

void ABaseCharacter::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("Interact called"));
	
	if (CurrentVehicle)
	{
		ExitVehicle();
		return;
	}

	AActor* Interactable = GetInteractableActor();
	if (Interactable && Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Interacting with: %s"), *Interactable->GetName());
		IInteractable::Execute_OnInteract(Interactable, this);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No interactable actor found"));
	}
}

void ABaseCharacter::CreateCrosshairWidget()
{
	if (CrosshairWidgetClass && !CreatedCrosshairWidget)
	{
		CreatedCrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
		if (CreatedCrosshairWidget)
		{
			CreatedCrosshairWidget->AddToViewport();
			// По умолчанию показываем кроссхер
			CreatedCrosshairWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABaseCharacter::CreateInteractionWidget()
{
	if (InteractionWidgetClass && !CreatedInteractionWidget)
	{
		CreatedInteractionWidget = CreateWidget<UUserWidget>(GetWorld(), InteractionWidgetClass);
		if (CreatedInteractionWidget)
		{
			CreatedInteractionWidget->AddToViewport();
			CreatedInteractionWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABaseCharacter::ShowCrosshair(bool bShow)
{
	if (CreatedCrosshairWidget)
	{
		CreatedCrosshairWidget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void ABaseCharacter::ShowInteractionPrompt(bool bShow, const FText& PromptText)
{
	CreatedInteractionWidget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void ABaseCharacter::StartChoppingMinigame()
{
	// Implemented in Blueprint
}

void ABaseCharacter::OnFatigueStateChanged(EFatigueState State)
{
}

void ABaseCharacter::OnDrunkStateChanged(EDrunkState State)
{
}