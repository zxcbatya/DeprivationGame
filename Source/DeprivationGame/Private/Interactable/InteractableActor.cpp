#include "Interactable/InteractableActor.h"
#include "BaseCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	InteractionDistance = 500.0f;
	bCanInteract = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AInteractableActor::CanInteract_Implementation(APawn* InteractingPawn) const
{
	if (!bCanInteract || !InteractingPawn)
	{
		return false;
	}

	return true;
}

void AInteractableActor::OnInteract_Implementation(APawn* InteractingPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("BASE InteractableActor OnInteract called for: %s"), *GetName());
}

void AInteractableActor::SetCanInteract(bool bNewCanInteract)
{
	bCanInteract = bNewCanInteract;
}

void AInteractableActor::SetInteractionText(const FText& NewText)
{
	InteractionText = NewText;
}

void AInteractableActor::SetInteractionDistance(float NewDistance)
{
	InteractionDistance = NewDistance;
}

FText AInteractableActor::GetInteractionText_Implementation() const
{
	return InteractionText;
}

float AInteractableActor::GetInteractionDistance_Implementation() const
{
	return InteractionDistance;
}


