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
	
	// Log for debugging
	UE_LOG(LogTemp, Log, TEXT("Axe interacted with pawn: %s"), InteractingPawn ? *InteractingPawn->GetName() : TEXT("nullptr"));
	
	// Find the ChopWoodGame component on the interacting pawn
	UChopWoodGame* ChopWoodComponent = InteractingPawn->FindComponentByClass<UChopWoodGame>();
	if (ChopWoodComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("Found ChopWoodGame component"));
		// Start the minigame through the component
		ChopWoodComponent->StartMinigameFromActor(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ChopWoodGame component not found on pawn"));
	}
}

bool AAxe::CanInteract_Implementation(APawn* InteractingPawn) const
{
	return bCanInteract && InteractingPawn != nullptr;
}