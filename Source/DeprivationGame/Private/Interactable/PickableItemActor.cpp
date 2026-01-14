#include "Interactable/PickableItemActor.h"
#include "Interactable/PlacementZone.h"
#include "GameFramework/Pawn.h"
#include "BaseCharacter.h"

APickableItemActor::APickableItemActor()
{
	InteractionText = FText::FromString(TEXT("Pick Up"));
	InteractionDistance = 200.0f;
	bCanInteract = true;
	bIsHeld = false;
	bCannotBePickedUpAgain = false;
}

void APickableItemActor::BeginPlay()
{
	Super::BeginPlay();
	SaveOriginalTransform();
}

bool APickableItemActor::CanInteract_Implementation(APawn* InteractingPawn) const
{
	// Базовая проверка
	if (!Super::CanInteract_Implementation(InteractingPawn))
	{
		return false;
	}

	// Нельзя взять предмет, если он уже в руке
	if (bIsHeld)
	{
		return false;
	}

	// Проверяем флаг невозможности повторного подбора
	if (bCannotBePickedUpAgain)
	{
		return false;
	}

	return true;
}

void APickableItemActor::OnInteract_Implementation(APawn* InteractingPawn)
{
	if (!CanInteract_Implementation(InteractingPawn))
	{
		return;
	}

	Super::OnInteract_Implementation(InteractingPawn);

	// Пытаемся взять предмет в руку через персонажа
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
	{
		Character->PickUpItem(this);
	}

	ReceiveItemPicked(InteractingPawn);
}

void APickableItemActor::SaveOriginalTransform()
{
	OriginalLocation = GetActorLocation();
	OriginalRotation = GetActorRotation();
}

void APickableItemActor::ReturnToOriginalPosition()
{
	if (!bIsHeld) return;

	// Отключаем прикрепление
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	// Возвращаем коллизию
	SetActorEnableCollision(true);
	
	// Возвращаем предмет на исходную позицию
	SetActorLocation(OriginalLocation);
	SetActorRotation(OriginalRotation);
	
	// Показываем предмет (на случай если был скрыт)
	SetActorHiddenInGame(false);
	
	bIsHeld = false;
}

bool APickableItemActor::IsValidPlacementZone(APlacementZone* Zone) const
{
	if (!Zone || ValidPlacementZones.Num() == 0)
	{
		return true; // Если список пуст, разрешаем везде
	}
	
	// Проверяем, есть ли зона в списке валидных
	for (TSubclassOf<APlacementZone> ValidZoneClass : ValidPlacementZones)
	{
		if (Zone->IsA(ValidZoneClass))
		{
			return true;
		}
	}
	
	return false;
}