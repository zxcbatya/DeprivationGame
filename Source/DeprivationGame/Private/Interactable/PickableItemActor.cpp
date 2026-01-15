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
	return true;
}

void APickableItemActor::OnInteract_Implementation(APawn* InteractingPawn)
{
	if (!CanInteract_Implementation(InteractingPawn))
	{
		return;
	}

	Super::OnInteract_Implementation(InteractingPawn);

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

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	SetActorEnableCollision(true);
	
	SetActorLocation(OriginalLocation);
	SetActorRotation(OriginalRotation);
	
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