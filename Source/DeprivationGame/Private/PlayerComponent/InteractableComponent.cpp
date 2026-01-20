#include "PlayerComponent/InteractableComponent.h"
#include "BaseCharacter.h"
#include "Interactable/PickableItemActor.h"
#include "Interactable/BackpackActor.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/IWidgetAnimationHandler.h"
#include "Kismet/GameplayStatics.h"

UInteractableComponent::UInteractableComponent(): CreatedCrosshairWidget(nullptr), 
                                                  CharacterOwner(nullptr),
                                                  ItemHoldSocket(nullptr), HeldItem(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanInteract = true;
	CurrentHoveredInteractable = nullptr;
	bIsCrosshairVisible = false;
	LastLineTraceTime = 0.0f;
	static ConstructorHelpers::FClassFinder<UUserWidget> CrosshairFinder(
		TEXT("/Game//BluePrints/PlayerWidgets/InteractWidget/WBP_Crosshair"));
	if (CrosshairFinder.Succeeded())
	{
		CrosshairWidgetClass = CrosshairFinder.Class;
	}
	
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterOwner = Cast<ABaseCharacter>(GetOwner());
	CreateWidgets();
	if (CharacterOwner)
	{
		ItemHoldSocket = CharacterOwner->ItemHoldSocket;
		HeldItem = CharacterOwner->blsHoldItem;
	}

	GetWorld()->GetTimerManager().SetTimer(InteractionCheckTimerHandle, this, &UInteractableComponent::CheckInteraction,
	                                       0.1f, true);
}

void UInteractableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(InteractionCheckTimerHandle);
}

void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractableComponent::CreateWidgets()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CreatedCrosshairWidget = CreateWidget<UUserWidget>(PC, CrosshairWidgetClass);
	if (CreatedCrosshairWidget)
	{
		CreatedCrosshairWidget->AddToViewport();
		CreatedCrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}


EInteractionType UInteractableComponent::DetermineInteractionType(AActor* Interactable)
{
	if (Interactable->GetClass()->IsChildOf(ABackpackActor::StaticClass()))
	{
		return EInteractionType::Container;
	}
	if (Interactable->GetClass()->IsChildOf(APickableItemActor::StaticClass()))
	{
		return EInteractionType::Pickable;
	}
	return EInteractionType::Standard;
}

AActor* UInteractableComponent::GetInteractableActor() const
{
	if (!CharacterOwner) return nullptr;
	
	if (CharacterOwner->IsInVehicle())
	{
		return reinterpret_cast<AActor*>(CharacterOwner->CurrentVehicle);
	}

	const float MaxInteractionDistance = 350.0f;
	AActor* HitActor = LineTrace(MaxInteractionDistance, false);

	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		float Dist = FVector::Dist(CharacterOwner->GetActorLocation(), HitActor->GetActorLocation());
		float InteractionDistance = IInteractable::Execute_GetInteractionDistance(HitActor);

		if (IInteractable::Execute_CanInteract(HitActor, CharacterOwner) && Dist <= InteractionDistance)
		{
			return HitActor;
		}
	}
	return nullptr;
}

void UInteractableComponent::Interact()
{
	if (!CharacterOwner) return;
	
	bCanInteract = false;

	AActor* Interactable = GetInteractableActor();
	if (!Interactable) return;

	GetWorld()->GetTimerManager().SetTimer(InteractDebounceTimerHandle, [this]()
	{
		bCanInteract = true;
	}, 0.3f, false);

	EInteractionType InteractionType = DetermineInteractionType(Interactable);

	OnInteractionStarted.Broadcast(Interactable, CharacterOwner);

	ProcessInteractionByType(Interactable, InteractionType);
}

void UInteractableComponent::CheckInteraction()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastLineTraceTime < 0.1f) return;
	LastLineTraceTime = CurrentTime;

	AActor* NewHoveredInteractable = GetInteractableActor();

	bool bIsValidInteractable =
		IsValid(NewHoveredInteractable) &&
		NewHoveredInteractable->GetClass()->ImplementsInterface(UInteractable::StaticClass()) &&
		IInteractable::Execute_CanInteract(NewHoveredInteractable, CharacterOwner);

	CurrentHoveredInteractable = bIsValidInteractable ? NewHoveredInteractable : nullptr;

	if (CharacterOwner->CurrentHoveredInteractable != CurrentHoveredInteractable.Get())
	{
		CharacterOwner->CurrentHoveredInteractable = CurrentHoveredInteractable.Get();
	}

	if (bIsValidInteractable)
	{
		FText InteractionText = FText::GetEmpty();

		if (NewHoveredInteractable->Implements<UInteractable>())
		{
			InteractionText = IInteractable::Execute_GetInteractionText(NewHoveredInteractable);
		}

		ShowCrosshair(true);
	}
	else
	{
		ShowCrosshair(false);
	}
}

void UInteractableComponent::ProcessInteractionByType(AActor* Interactable, EInteractionType Type)
{
	switch (Type)
	{
	case EInteractionType::Container:
		HandleContainerInteraction(Interactable);
		break;

	case EInteractionType::Pickable:
		HandlePickableInteraction(Interactable);
		break;

	case EInteractionType::Standard:
	default:
		HandleStandardInteraction(Interactable);
		break;
	}
}

void UInteractableComponent::HandleContainerInteraction(AActor* Container)
{
	if (Container && Container->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_OnInteract(Container, CharacterOwner);
		OnInteractionCompleted.Broadcast(Container, CharacterOwner);
	}
}

void UInteractableComponent::HandlePickableInteraction(AActor* Pickable)
{
	if (APickableItemActor* PickableItem = Cast<APickableItemActor>(Pickable))
	{
		PickUpItem(PickableItem);
		OnInteractionCompleted.Broadcast(Pickable, CharacterOwner);
	}
}

void UInteractableComponent::HandleStandardInteraction(AActor* Interactable)
{
	if (Interactable && Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_OnInteract(Interactable, CharacterOwner);
		OnInteractionCompleted.Broadcast(Interactable, CharacterOwner);
	}
}

void UInteractableComponent::PickUpItem(APickableItemActor* ItemToPick)
{
	if (!CharacterOwner || !ItemHoldSocket) return;

	CharacterOwner->PickUpItem(ItemToPick);
	HeldItem = ItemToPick;
}

APickableItemActor* UInteractableComponent::GetHoldItem() const
{
	return HeldItem;
}

void UInteractableComponent::SetHoldItem(APickableItemActor* NewItem)
{
	HeldItem = NewItem;
}

void UInteractableComponent::DropItem()
{
	if (!CharacterOwner || !HeldItem) return;
	
	HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	HeldItem->SetActorEnableCollision(true);
	HeldItem->SetActorHiddenInGame(false);
	HeldItem->SetIsHeld(false);
	HeldItem->ReturnToOriginalPosition();
	
	HeldItem = nullptr;
	
	if (CharacterOwner)
	{
		CharacterOwner->blsHoldItem = nullptr;
	}
}

void UInteractableComponent::ShowCrosshair(bool bShow)
{
	if (bShow == bIsCrosshairVisible) return;
	
	bIsCrosshairVisible = bShow;
	
	if (CreatedCrosshairWidget)
	{
		CreatedCrosshairWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		if (bShow)
		{
			if (CreatedCrosshairWidget->GetClass()->ImplementsInterface(UWidgetAnimationHandler::StaticClass()))
			{
				IWidgetAnimationHandler::Execute_PlayShowAnimation(CreatedCrosshairWidget);
			}
		}
		else
		{
			if (CreatedCrosshairWidget->GetClass()->ImplementsInterface(UWidgetAnimationHandler::StaticClass()))
			{
				IWidgetAnimationHandler::Execute_PlayHideAnimation(CreatedCrosshairWidget);
			}
		}
	}
}


AActor* UInteractableComponent::LineTrace(float LineLength, bool bDrawDebug) const
{
	UCameraComponent* CameraComponent = CharacterOwner->GetCameraComponent();
	FVector Start = CameraComponent ? CameraComponent->GetComponentLocation() : CharacterOwner->GetActorLocation();
	FVector End = Start + (CameraComponent
		                       ? CameraComponent->GetForwardVector()
		                       : CharacterOwner->GetActorForwardVector()) * LineLength;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CharacterOwner);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (bDrawDebug)
	{
		FColor Color = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), Start, End, Color, false, 2.0f, 0, 2.0f);
		if (bHit) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, Color, false, 2.0f);
	}

	return bHit ? Hit.GetActor() : nullptr;
}
