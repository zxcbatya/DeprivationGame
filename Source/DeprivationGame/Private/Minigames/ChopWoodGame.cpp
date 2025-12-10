#include "Minigames/ChopWoodGame.h"
#include "Minigames/ChopWoodWidget.h"
#include "Interfaces/IChopWoodGameListener.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

UChopWoodGame::UChopWoodGame()
{
	PrimaryComponentTick.bCanEverTick = true;
	InitializeDefaults();
}

void UChopWoodGame::InitializeDefaults()
{
	TargetZoneSize = 0.2f;
	IndicatorSpeed = 1.0f;
	IsChopping = false;
	IndicatorPosition = 0.0f;
	IndicatorDirection = true;
	ChopsSuccessful = 0;
	ChopsFailed = 0;
	RequiredHits = 3;
	TargetZoneCenter = 0.5f;
	CurrentTargetZoneSize = 0.2f;
	TargetZoneShrinkStep = 0.05f;
	MinTargetZoneSize = 0.05f;
	InitialTargetZoneSize = 0.2f;
	InitialTargetZoneCenter = 0.5f;
	bWasRotationInputAllowed = true; // Default rotation allowed

	// Initialize new variables
	bWasClickEventsAllowed = true;
	bWasMouseOverEventsAllowed = true;
	bWasLookInputAllowed = true;
	bWasMoveInputAllowed = true;
	WidgetDisplayDelay = 0.5f; // Default delay of 0.5 seconds
}

void UChopWoodGame::BeginPlay()
{
	Super::BeginPlay();
}

void UChopWoodGame::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		BindInputActions(EnhancedInput);
	}
}

void UChopWoodGame::BindInputActions(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ChopAction)
		{
			EnhancedInput->BindAction(ChopAction, ETriggerEvent::Started, this, &UChopWoodGame::HandleChopInput);
		}
	}
}

void UChopWoodGame::HandleChopInput(const FInputActionValue& Value)
{
	AttemptChop();
}

void UChopWoodGame::UpdateWidget()
{
	if (CreatedWidget.IsValid())
	{
		CreatedWidget->CurrentTargetZoneSize = CurrentTargetZoneSize;
		CreatedWidget->TargetZoneCenter = TargetZoneCenter;
		CreatedWidget->UpdateVisuals();
	}
}

void UChopWoodGame::NotifyGameCompleted(bool bSuccess)
{
	if (AActor* Owner = GetOwner())
	{
		// Notify using the IChopWoodGameListener interface
		if (Owner->GetClass()->ImplementsInterface(UChopWoodGameListener::StaticClass()))
		{
			if (bSuccess)
			{
				IChopWoodGameListener::Execute_OnChopWoodGameCompleted(Owner);
			}
			else
			{
				IChopWoodGameListener::Execute_OnChopWoodGameFailed(Owner);
			}
		}
	}
}

void UChopWoodGame::NotifyHitResult(bool bSuccess)
{
	if (AActor* Owner = GetOwner())
	{
		// Notify using the IChopWoodGameListener interface
		if (Owner->GetClass()->ImplementsInterface(UChopWoodGameListener::StaticClass()))
		{
			if (bSuccess)
			{
				IChopWoodGameListener::Execute_OnChopWoodHitSuccess(Owner, ChopsSuccessful);
				
				// Trigger animation event
				IChopWoodGameListener::Execute_PlayChopSuccessAnimation(Owner);
			}
			else
			{
				IChopWoodGameListener::Execute_OnChopWoodHitFailed(Owner, ChopsFailed);
				
				// Trigger animation event
				IChopWoodGameListener::Execute_PlayChopFailAnimation(Owner);
			}
		}
	}
}

void UChopWoodGame::SavePlayerInputState(APlayerController* PC)
{
	if (!PC) return;

	// Save the complete input state
	bWasClickEventsAllowed = PC->bEnableClickEvents;
	bWasMouseOverEventsAllowed = PC->bEnableMouseOverEvents;
	bWasLookInputAllowed = PC->bEnableClickEvents; // Look input is tied to click events in many cases
	bWasMoveInputAllowed = !PC->IsMoveInputIgnored();
}

void UChopWoodGame::RestorePlayerInputState(APlayerController* PC)
{
	if (!PC) return;

	// Restore the complete input state
	PC->bEnableClickEvents = bWasClickEventsAllowed;
	PC->bEnableMouseOverEvents = bWasMouseOverEventsAllowed;

	// Restore movement if it was enabled
	if (bWasMoveInputAllowed)
	{
		PC->SetIgnoreMoveInput(false);
	}
}

void UChopWoodGame::DisableAllPlayerInput(APlayerController* PC)
{
	if (!PC) return;

	// Completely disable all player input
	PC->bEnableClickEvents = false;
	PC->bEnableMouseOverEvents = false;
	PC->SetIgnoreLookInput(true);
	PC->SetIgnoreMoveInput(true);
}

void UChopWoodGame::EnableAllPlayerInput(APlayerController* PC)
{
	if (!PC) return;

	// Re-enable all player input
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;
	PC->SetIgnoreLookInput(false);
	PC->SetIgnoreMoveInput(false);
}

void UChopWoodGame::StartChopping()
{
	IsChopping = true;
	IndicatorPosition = 0.0f;
	IndicatorDirection = true;
	ChopsSuccessful = 0;
	ChopsFailed = 0;
	CurrentTargetZoneSize = InitialTargetZoneSize;
	TargetZoneCenter = InitialTargetZoneCenter;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		if (APawn* Pawn = Cast<APawn>(Owner))
		{
			if (ACharacter* Character = Cast<ACharacter>(Pawn))
			{
				Character->GetCharacterMovement()->DisableMovement();
				Character->GetCharacterMovement()->StopMovementImmediately();

				if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
				{
					// Save the complete input state
					SavePlayerInputState(PC);

					// Completely disable all input during gameplay
					DisableAllPlayerInput(PC);

					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
						UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
					{
						if (ChopWoodMappingContext)
						{
							Subsystem->AddMappingContext(ChopWoodMappingContext, 0);
						}
					}

					if (UInputComponent* InputComponent = PC->InputComponent)
					{
						BindInputActions(InputComponent);
					}
				}
			}
		}
	}

	if (ChopWoodWidgetClass && !CreatedWidget.IsValid())
	{
		if (UWorld* World = GetWorld())
		{
			UChopWoodWidget* NewWidget = CreateWidget<UChopWoodWidget>(World, ChopWoodWidgetClass);
			CreatedWidget = MakeWeakObjectPtr(NewWidget);
			if (CreatedWidget.IsValid())
			{
				CreatedWidget->AddToViewport();
				
				// Notify owner to play start animation
				if (AActor* Owner = GetOwner())
				{
					if (Owner->GetClass()->ImplementsInterface(UChopWoodGameListener::StaticClass()))
					{
						IChopWoodGameListener::Execute_PlayChopStartAnimation(Owner);
					}
				}
			}
		}
	}

	UpdateWidget();
}

void UChopWoodGame::StopChopping()
{
	IsChopping = false;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		if (APawn* Pawn = Cast<APawn>(Owner))
		{
			if (ACharacter* Character = Cast<ACharacter>(Pawn))
			{
				Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

				if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
				{
					// Fully restore all input
					EnableAllPlayerInput(PC);

					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
						UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
					{
						if (ChopWoodMappingContext)
						{
							Subsystem->RemoveMappingContext(ChopWoodMappingContext);
						}
					}

					// Note: Enhanced Input bindings are automatically cleaned up when 
					// the mapping context is removed, so we don't need to manually unbind
				}
			}
		}
	}

	// Safely remove widget with proper memory management
	if (CreatedWidget.IsValid())
	{
		CreatedWidget->RemoveFromParent();
		CreatedWidget.Reset(); // Explicitly reset the weak pointer
	}

	// Clear any pending timers
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(WidgetDisplayTimerHandle);
	}
}

bool UChopWoodGame::AttemptChop()
{
	if (!IsChopping)
	{
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Chop attempt - Indicator Position: %f"), IndicatorPosition);
	UE_LOG(LogTemp, Log, TEXT("Target Zone: Start=%f, End=%f"),
	       TargetZoneCenter - CurrentTargetZoneSize * 0.5f,
	       TargetZoneCenter + CurrentTargetZoneSize * 0.5f);

	const float ZoneStart = FMath::Max(0.0f, TargetZoneCenter - CurrentTargetZoneSize * 0.5f);
	const float ZoneEnd = FMath::Min(1.0f, TargetZoneCenter + CurrentTargetZoneSize * 0.5f);

	constexpr float Tolerance = 0.02f;

	if (IndicatorPosition >= (ZoneStart - Tolerance) && IndicatorPosition <= (ZoneEnd + Tolerance))
	{
		ChopsSuccessful++;
		UE_LOG(LogTemp, Log, TEXT("Chop SUCCESSFUL - Count: %d"), ChopsSuccessful);
		
		// Play success animation
		if (CreatedWidget.IsValid())
		{
			CreatedWidget->PlaySuccessAnimation();
		}
		
		// Notify hit result and trigger animation
		NotifyHitResult(true);

		if (ChopsSuccessful >= RequiredHits)
		{
			NotifyGameCompleted(true);
			StopChopping();
			return true;
		}

		TargetZoneCenter = FMath::RandRange(CurrentTargetZoneSize * 0.5f, 1.0f - CurrentTargetZoneSize * 0.5f);

		CurrentTargetZoneSize = FMath::Max(MinTargetZoneSize, CurrentTargetZoneSize * 0.5f);

		UpdateWidget();

		return true;
	}
	else
	{
		ChopsFailed++;
		UE_LOG(LogTemp, Log, TEXT("Chop FAILED - Count: %d"), ChopsFailed);
		
		// Play failure animation
		if (CreatedWidget.IsValid())
		{
			CreatedWidget->PlayFailureAnimation();
		}
		
		// Notify hit result and trigger animation
		NotifyHitResult(false);

		if (ChopsFailed >= 3)
		{
			NotifyGameCompleted(false);
			StopChopping();
		}

		return false;
	}
}

void UChopWoodGame::StartMinigameFromActor(AActor* InteractingActor)
{
	if (InteractingActor)
	{
		FString ActorClassName = InteractingActor->GetClass()->GetName();
		if (ActorClassName.Contains("Axe"))
		{
			StartChopping();
		}
	}
}

void UChopWoodGame::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsChopping)
		return;

	if (IndicatorDirection)
	{
		IndicatorPosition += IndicatorSpeed * DeltaTime;
		if (IndicatorPosition >= 1.0f)
		{
			IndicatorPosition = 1.0f;
			IndicatorDirection = false;
		}
	}
	else
	{
		IndicatorPosition -= IndicatorSpeed * DeltaTime;
		if (IndicatorPosition <= 0.0f)
		{
			IndicatorPosition = 0.0f;
			IndicatorDirection = true;
		}
	}

	// Update the indicator position in the widget
	if (CreatedWidget.IsValid())
	{
		CreatedWidget->IndicatorPosition = IndicatorPosition;
		CreatedWidget->IndicatorDirection = IndicatorDirection;
		CreatedWidget->UpdateVisuals();
	}
}