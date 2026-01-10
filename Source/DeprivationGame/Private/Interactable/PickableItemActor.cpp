#include "Interactable/PickableItemActor.h"
#include "GameFramework/Pawn.h"

APickableItemActor::APickableItemActor()
{
	InteractionText = FText::FromString(TEXT("Pick Up"));
	InteractionDistance = 200.0f;
	bCanInteract = true;
}

void APickableItemActor::BeginPlay()
{
	Super::BeginPlay();
}

void APickableItemActor::OnInteract_Implementation(APawn* InteractingPawn)
{
	if (!CanInteract_Implementation(InteractingPawn))
	{
		return;
	}

	Super::OnInteract_Implementation(InteractingPawn);
}

