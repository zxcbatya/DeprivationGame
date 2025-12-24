#include "Interactable/Axe.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "Minigames/ChopWoodGame.h"

AAxe::AAxe()
{
	PrimaryActorTick.bCanEverTick = false;
	InteractionText = FText::FromString(TEXT("Take Axe"));
	InteractionDistance = 200.0f;
	bCanInteract = true;
}

void AAxe::BeginPlay()
{
	Super::BeginPlay();
}

void AAxe::OnInteract_Implementation(APawn* InteractingPawn)
{
	Super::OnInteract_Implementation(InteractingPawn);

	UChopWoodGame* ChopWoodComponent = InteractingPawn->FindComponentByClass<UChopWoodGame>();

	ChopWoodComponent->StartMinigameFromActor(this);
}

bool AAxe::CanInteract_Implementation(APawn* InteractingPawn) const
{
	return bCanInteract && InteractingPawn != nullptr;
}
