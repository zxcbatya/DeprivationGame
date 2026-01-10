#include "Interactable/PlacementZone.h"
#include "Interactable/PickableItemActor.h"
#include "BaseCharacter.h"
#include "GameFramework/Pawn.h"

APlacementZone::APlacementZone()
{
	InteractionText = FText::FromString(TEXT("Place Item"));
	InteractionDistance = 200.0f;
	bCanInteract = true;
	bCanPlaceItem = true;
	bSingleItemOnly = true;
	PlacedItem = nullptr;

	// Создаем сокет для размещения предмета
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
	// Базовая проверка
	if (!Super::CanInteract_Implementation(InteractingPawn))
	{
		return false;
	}

	// PlacementZone можно использовать только если персонаж держит предмет
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
	{
		if (!Character->GetHoldItem())
		{
			return false; // Нет предмета в руке
		}

		// Проверяем, можно ли разместить предмет в этой зоне
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

	Super::OnInteract_Implementation(InteractingPawn);

	// Если это персонаж с предметом в руке, пытаемся разместить предмет
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
	{
		if (APickableItemActor* HeldItem = Character->GetHoldItem())
		{
			if (CanPlaceItem(HeldItem))
			{
				PlaceItem(HeldItem);
				Character->SetHoldItem(nullptr); // Персонаж больше не держит предмет
			}
		}
	}
}

bool APlacementZone::PlaceItem(APickableItemActor* ItemToPlace)
{
	if (!ItemToPlace || !bCanPlaceItem) return false;

	// Если зона позволяет только один предмет и уже есть предмет
	if (bSingleItemOnly && PlacedItem && PlacedItem != ItemToPlace)
	{
		return false;
	}

	// Убираем предмет из руки персонажа (если был в руке)
	if (ItemToPlace->IsHeld())
	{
		ItemToPlace->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ItemToPlace->SetIsHeld(false);
	}

	// Размещаем предмет в зоне
	PlacedItem = ItemToPlace;
	ItemToPlace->SetActorEnableCollision(true);
	ItemToPlace->AttachToComponent(PlacementSocket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ItemToPlace->SetActorHiddenInGame(false);

	// Сохраняем новую позицию как исходную для предмета
	ItemToPlace->SaveOriginalTransform();

	OnItemPlaced(ItemToPlace);
	return true;
}

void APlacementZone::RemoveItem()
{
	if (!PlacedItem) return;

	APickableItemActor* ItemToRemove = PlacedItem;
	PlacedItem = nullptr;

	OnItemRemoved(ItemToRemove);
}

bool APlacementZone::CanPlaceItem(APickableItemActor* ItemToPlace) const
{
	if (!ItemToPlace || !bCanPlaceItem) return false;

	// Если зона позволяет только один предмет и уже есть другой предмет
	if (bSingleItemOnly && PlacedItem && PlacedItem != ItemToPlace)
	{
		return false;
	}

	return true;
}

FText APlacementZone::GetInteractionText_Implementation() const
{
	// Если зона занята другим предметом, не показываем текст взаимодействия
	// (CanInteract уже вернет false, но на всякий случай)
	if (bSingleItemOnly && PlacedItem)
	{
		return FText::FromString(TEXT("Zone Occupied"));
	}

	// Возвращаем стандартный текст для размещения
	return InteractionText; // "Place Item"
}

