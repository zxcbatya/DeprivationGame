#include "Interactable/PlacementZone.h"
#include "Interactable/PickableItemActor.h"
#include "Interactable/BackpackActor.h"
#include "BaseCharacter.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

APlacementZone::APlacementZone()
{
	InteractionText = FText::FromString(TEXT("Place Item"));
	InteractionDistance = 200.0f;
	bCanInteract = true;
	bCanPlaceItem = true;
	bSingleItemOnly = true;
	PlacedItem = nullptr;

	PlacementSocket = CreateDefaultSubobject<USceneComponent>(TEXT("PlacementSocket"));
	PlacementSocket->SetupAttachment(RootComponent);
	PlacementSocket->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

void APlacementZone::BeginPlay()
{
	Super::BeginPlay();
}

bool APlacementZone::CanInteract_Implementation(APawn* InteractingPawn) const
{

	if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
	{
		if (!Character->GetHoldItem())
		{
			return false; // Нет предмета в руке
		}

		return CanPlaceItem(Character->GetHoldItem());
	}

	return false;
}

void APlacementZone::OnInteract_Implementation(APawn* InteractingPawn)
{
	if (!CanInteract_Implementation(InteractingPawn))
	{
		return;
	}
	PlaySound();
	Super::OnInteract_Implementation(InteractingPawn);

	if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
	{
		if (APickableItemActor* HeldItem = Character->GetHoldItem())
		{
			if (CanPlaceItem(HeldItem))
			{
				PlaceItem(HeldItem);
				Character->SetHoldItem(nullptr); 
			}
		}
	}
}

bool APlacementZone::PlaceItem(APickableItemActor* ItemToPlace)
{
	if (ItemToPlace->IsHeld())
	{
		ItemToPlace->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ItemToPlace->SetIsHeld(false);
	}

	PlacedItem = ItemToPlace;
	ItemToPlace->SetActorEnableCollision(true);
	ItemToPlace->AttachToComponent(PlacementSocket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ItemToPlace->SetActorHiddenInGame(false);

	ItemToPlace->SaveOriginalTransform();

	if (ItemToPlace->IsA(ABackpackActor::StaticClass()))
	{
		ABackpackActor* Backpack = Cast<ABackpackActor>(ItemToPlace);
		if (Backpack)
		{
			Backpack->SetPlacedAsContainer(true);
		}
	}
	else
	{
		ItemToPlace->SetCannotBePickedUpAgain(true);
	}

	OnItemPlaced(ItemToPlace);
	return true;
}

void APlacementZone::RemoveItem()
{
	APickableItemActor* ItemToRemove = PlacedItem;
	PlacedItem = nullptr;

	OnItemRemoved(ItemToRemove);
}

bool APlacementZone::CanPlaceItem(APickableItemActor* ItemToPlace) const
{
	if (bSingleItemOnly && PlacedItem && PlacedItem != ItemToPlace)
	{
		return false;
	}

	if (!ItemToPlace->IsValidPlacementZone(const_cast<APlacementZone*>(this)))
	{
		return false;
	}

	return true;
}

FText APlacementZone::GetInteractionText_Implementation() const
{
	if (bSingleItemOnly && PlacedItem)
	{
		return FText::FromString(TEXT("Zone Occupied"));
	}

	// Возвращаем стандартный текст для размещения
	return InteractionText; // "Place Item"
}

