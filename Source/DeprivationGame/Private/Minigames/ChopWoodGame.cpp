#include "Minigames/ChopWoodGame.h"
#include "Minigames/ChopWoodWidget.h"
#include "Interfaces/IChopWoodGameListener.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Components/CanvasPanelSlot.h"

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
		CreatedWidget->UpdateVisuals(TargetZoneCenter, CurrentTargetZoneSize, IndicatorPosition);
	}
}

void UChopWoodGame::NotifyGameCompleted(bool bSuccess)
{
	if (AActor* Owner = GetOwner())
	{
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
		if (Owner->GetClass()->ImplementsInterface(UChopWoodGameListener::StaticClass()))
		{
			if (bSuccess)
			{
				IChopWoodGameListener::Execute_OnChopWoodHitSuccess(Owner, ChopsSuccessful);

				IChopWoodGameListener::Execute_PlayChopSuccessAnimation(Owner);
			}
			else
			{
				IChopWoodGameListener::Execute_OnChopWoodHitFailed(Owner, ChopsFailed);

				IChopWoodGameListener::Execute_PlayChopFailAnimation(Owner);
			}
		}
	}
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
					PC->bEnableClickEvents = false;
					PC->bEnableMouseOverEvents = false;
					PC->SetIgnoreLookInput(true);
					PC->SetIgnoreMoveInput(true);

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
				if (AActor* OwnerActor = GetOwner())
				{
					if (OwnerActor->GetClass()->ImplementsInterface(UChopWoodGameListener::StaticClass()))
					{
						IChopWoodGameListener::Execute_PlayChopStartAnimation(OwnerActor);
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
					// Enable all player input
					PC->bEnableClickEvents = true;
					PC->bEnableMouseOverEvents = true;
					PC->SetIgnoreLookInput(false);
					PC->SetIgnoreMoveInput(false);

					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
						UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
					{
						if (ChopWoodMappingContext)
						{
							Subsystem->RemoveMappingContext(ChopWoodMappingContext);
						}
					}
				}
			}
		}
	}

	if (CreatedWidget.IsValid())
	{
		CreatedWidget->RemoveFromParent();
		CreatedWidget.Reset();
	}

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

	// Обновляем визуал — чтобы позиции были актуальны
	UpdateWidget();

	// Получаем виджет
	if (!CreatedWidget.IsValid())
	{
		return false;
	}

	// Получаем слоты
	UCanvasPanelSlot* ZoneSlot = Cast<UCanvasPanelSlot>(CreatedWidget->TargetZoneImage->Slot);
	UCanvasPanelSlot* IndicatorSlot = Cast<UCanvasPanelSlot>(CreatedWidget->IndicatorImage->Slot);

	if (!ZoneSlot || !IndicatorSlot)
	{
		return false;
	}

	// Получаем позиции и размеры в локальных координатах CanvasPanel
	FVector2D ZonePos = ZoneSlot->GetPosition();
	FVector2D ZoneSize = ZoneSlot->GetSize();

	FVector2D IndicatorPos = IndicatorSlot->GetPosition();
	FVector2D IndicatorSize = IndicatorSlot->GetSize();

	// Вычисляем границы
	float ZoneLeft = ZonePos.X;
	float ZoneRight = ZonePos.X + ZoneSize.X;
	float ZoneTop = ZonePos.Y;
	float ZoneBottom = ZonePos.Y + ZoneSize.Y;

	float IndicatorLeft = IndicatorPos.X;
	float IndicatorRight = IndicatorPos.X + IndicatorSize.X;
	float IndicatorTop = IndicatorPos.Y;
	float IndicatorBottom = IndicatorPos.Y + IndicatorSize.Y;

	// Проверяем пересечение
	bool bIntersect =
		IndicatorLeft < ZoneRight &&
		IndicatorRight > ZoneLeft &&
		IndicatorTop < ZoneBottom &&
		IndicatorBottom > ZoneTop;

	if (bIntersect)
	{
		ChopsSuccessful++;
		if (CreatedWidget.IsValid())
		{
			CreatedWidget->PlaySuccessAnimation();
		}

		NotifyHitResult(true);

		if (ChopsSuccessful >= RequiredHits)
		{
			NotifyGameCompleted(true);
			StopChopping();
			return true;
		}

		// Смещаем зону и уменьшаем её
		TargetZoneCenter = FMath::RandRange(CurrentTargetZoneSize * 0.5f, 1.0f - CurrentTargetZoneSize * 0.5f);
		CurrentTargetZoneSize = FMath::Max(MinTargetZoneSize, CurrentTargetZoneSize * 0.5f);

		UpdateWidget(); // Обновляем UI после изменения зоны

		return true;
	}
	else
	{
		ChopsFailed++;
		if (CreatedWidget.IsValid())
		{
			CreatedWidget->PlayFailureAnimation();
		}

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

	if (CreatedWidget.IsValid())
	{
		CreatedWidget->UpdateVisuals(TargetZoneCenter, CurrentTargetZoneSize, IndicatorPosition);
	}
}
