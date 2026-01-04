#include "Interactable/DoorActor.h"
#include "GameFramework/Pawn.h"

ADoorActor::ADoorActor()
{
	InteractionText = FText::FromString(TEXT("Open Door"));
	InteractionDistance = 300.0f;
	bCanInteract = true;
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorActor::OnInteract_Implementation(APawn* InteractingPawn)
{
	if (!CanInteract_Implementation(InteractingPawn))
	{
		return;
	}

	Super::OnInteract_Implementation(InteractingPawn);
}

