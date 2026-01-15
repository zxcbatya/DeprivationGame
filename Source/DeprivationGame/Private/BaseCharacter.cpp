#include "BaseCharacter.h"
#include "Vehicle/DeprivationCar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/IInteractable.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Interfaces/IWidgetAnimationHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Interactable/PickableItemActor.h"
#include "Interactable/PlacementZone.h"
#include "Interactable/BackpackActor.h"

// Типы взаимодействия для event-driven архитектуры
enum class EInteractionType
{
	Container,   // Контейнеры (рюкзаки)
	Pickable,    // Подбираемые предметы
	Standard     // Стандартные интерактивные объекты
};

// Определение типа взаимодействия
EInteractionType DetermineInteractionType(AActor* Interactable)
{
	if (Interactable->GetClass()->IsChildOf(ABackpackActor::StaticClass()))
	{
		return EInteractionType::Container;
	}
	
	// Подбираемые предметы
	if (Interactable->GetClass()->IsChildOf(APickableItemActor::StaticClass()))
	{
		return EInteractionType::Pickable;
	}
	
	return EInteractionType::Standard;
}

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraOffset = FVector(0.0f, 15.0f, 170.0f);

	// Сначала создаем камеру
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetMesh());
	CameraComponent->SetRelativeLocation(CameraOffset);
	CameraComponent->bUsePawnControlRotation = true;
	
	// Затем создаем сокет для предметов и прикрепляем к камере
	ItemHoldSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ItemHoldSocket"));
	ItemHoldSocket->SetupAttachment(CameraComponent);
	ItemHoldSocket->SetRelativeLocation(FVector(50.0f, 20.0f, -20.0f)); // Смещение для держания перед камерой
	ItemHoldSocket->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

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

	// Start interaction check timer
	GetWorldTimerManager().SetTimer(InteractionCheckTimerHandle, this, &ABaseCharacter::CheckInteraction, 0.1f, true);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraComponent->SetRelativeLocation(CameraOffset);
}

void ABaseCharacter::CheckInteraction()
{
    AActor* NewHoveredInteractable = GetInteractableActor();
    
    static int32 SameObjectCounter = 0;
    static AActor* LastCheckedObject = nullptr;
    
    if (NewHoveredInteractable == LastCheckedObject)
    {
        SameObjectCounter++;
    }
    else
    {
        SameObjectCounter = 0;
        LastCheckedObject = NewHoveredInteractable;
    }
    
    if (SameObjectCounter >= 2 && NewHoveredInteractable != CurrentHoveredInteractable)
    {
        CurrentHoveredInteractable = NewHoveredInteractable;

        if (CurrentHoveredInteractable && CurrentHoveredInteractable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
        {
            FText InteractionText = IInteractable::Execute_GetInteractionText(CurrentHoveredInteractable);
            ShowCrosshair(true);
            
            if (CreatedInteractionWidget && CreatedInteractionWidget->GetClass()->ImplementsInterface(UWidgetAnimationHandler::StaticClass()))
            {
                IWidgetAnimationHandler::Execute_PlayShowAnimation(CreatedInteractionWidget);
            }
            if (CreatedCrosshairWidget && CreatedCrosshairWidget->GetClass()->ImplementsInterface(UWidgetAnimationHandler::StaticClass()))
            {
                IWidgetAnimationHandler::Execute_PlayShowAnimation(CreatedCrosshairWidget);
            }
        }
        else
        {
            ShowCrosshair(false);
            
            if (CreatedInteractionWidget && CreatedInteractionWidget->GetClass()->ImplementsInterface(UWidgetAnimationHandler::StaticClass()))
            {
                IWidgetAnimationHandler::Execute_PlayHideAnimation(CreatedInteractionWidget);
            }
            if (CreatedCrosshairWidget && CreatedCrosshairWidget->GetClass()->ImplementsInterface(UWidgetAnimationHandler::StaticClass()))
            {
                IWidgetAnimationHandler::Execute_PlayHideAnimation(CreatedCrosshairWidget);
            }
        }
    }
}

void ABaseCharacter::PickUpItem(APickableItemActor* ItemToPick)
{
	if (!ItemToPick) return;

	if (blsHoldItem)
	{
		blsHoldItem->ReturnToOriginalPosition();
		blsHoldItem = nullptr;
	}

	blsHoldItem = ItemToPick;
	
	// ВСЕГДА сохраняем трансформ при поднятии
	blsHoldItem->SaveOriginalTransform();
	
	blsHoldItem->SetIsHeld(true);
	blsHoldItem->SetActorEnableCollision(false);
	
	// Прикрепляем к общему сокету руки
	blsHoldItem->AttachToComponent(ItemHoldSocket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	// Применяем индивидуальное смещение и ротацию для каждого предмета
	blsHoldItem->SetActorRelativeLocation(blsHoldItem->GetHandOffset());
	blsHoldItem->SetActorRelativeRotation(blsHoldItem->GetHandRotation());
	
	blsHoldItem->SetActorHiddenInGame(false);
}

void ABaseCharacter::DropItem()
{
	if (!blsHoldItem) return;

	blsHoldItem->ReturnToOriginalPosition();
	blsHoldItem = nullptr;
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InteractionAction)
		{
			EnhancedInput->BindAction(InteractionAction, ETriggerEvent::Started, this, &ABaseCharacter::Interact);
		}

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
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

	// Store controller reference before unpossessing
	AController* MyController = GetController();
	
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->SetMovementMode(MOVE_None);
		MovementComp->StopMovementImmediately();
		MovementComp->SetComponentTickEnabled(false);
	}

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	AttachToComponent(Car->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// Handle controller possession
	if (MyController)
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

void ABaseCharacter::EnterVehicleByTag(FName VehicleTag)
{
	if (!GetWorld()) return;

	// Ищем все машины с указанным тегом
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), VehicleTag, FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ADeprivationCar* Car = Cast<ADeprivationCar>(Actor))
		{
			EnterVehicle(Car);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("BaseCharacter: Не найдена машина с тегом %s"), *VehicleTag.ToString());
}

void ABaseCharacter::EnterVehicleByReference(ADeprivationCar* Vehicle)
{
	if (Vehicle)
	{
		EnterVehicle(Vehicle);
	}
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

	const float MaxInteractionDistance = 1000.0f;
	AActor* HitActor = LineTrace(MaxInteractionDistance, false);
	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		float Dist = FVector::Dist(GetActorLocation(), HitActor->GetActorLocation());
		float InteractionDistance = IInteractable::Execute_GetInteractionDistance(HitActor);
		if (IInteractable::Execute_CanInteract(HitActor, const_cast<ABaseCharacter*>(this)) &&
			Dist <= InteractionDistance)
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
	if (!bCanInteract) return;
	
	if (CurrentVehicle)
	{
		ExitVehicle();
		return;
	}

	AActor* Interactable = GetInteractableActor();
	
	UE_LOG(LogTemp, Warning, TEXT("Interact pressed, found actor: %s"), 
		Interactable ? *Interactable->GetName() : TEXT("None"));
	
	if (Interactable)
	{
		// Проверяем интерфейс через Cast (работает с Blueprint)
		IInteractable* InteractableInterface = Cast<IInteractable>(Interactable);
		if (InteractableInterface)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found interactable actor: %s"), *Interactable->GetName());
		bCanInteract = false;
		GetWorldTimerManager().SetTimer(InteractDebounceTimerHandle, [this]() {
			bCanInteract = true;
		}, 0.3f, false);
		
		// Event-driven подход: определяем тип взаимодействия
		EInteractionType InteractionType = DetermineInteractionType(Interactable);
		
		switch (InteractionType)
		{
		case EInteractionType::Container:  // Рюкзаки и другие контейнеры
			UE_LOG(LogTemp, Warning, TEXT("EXECUTING OnInteract for CONTAINER")); 
			
			// Вызываем родительский метод - Blueprint может его переопределить
			IInteractable::Execute_OnInteract(Interactable, this);
			break;
			
		case EInteractionType::Pickable:  // Обычные подбираемые предметы
			if (APickableItemActor* PickableItem = Cast<APickableItemActor>(Interactable))
			{
				PickUpItem(PickableItem);
			}
			break;
			
		case EInteractionType::Standard:  // Двери, топоры и т.д.
		default:
			IInteractable::Execute_OnInteract(Interactable, this);
			break;
		}
		}
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
	if (CreatedInteractionWidget)
	{
		CreatedInteractionWidget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void ABaseCharacter::StartChoppingMinigame()
{
}

void ABaseCharacter::OnFatigueStateChanged(EFatigueState State)
{
}

void ABaseCharacter::OnDrunkStateChanged(EDrunkState State)
{
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear the interaction check timer to prevent delegate calls on destroyed object
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractionCheckTimerHandle);
	}

	// Hide any active interaction prompts
	if (CurrentHoveredInteractable && CurrentHoveredInteractable->GetClass()->ImplementsInterface(
		UInteractable::StaticClass()))
	{
		// Interaction ended for object
	}
}
