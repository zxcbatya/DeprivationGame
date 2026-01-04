#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "PickableItemActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class DEPRIVATIONGAME_API APickableItemActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	APickableItemActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickable Item")
	void ReceiveItemPicked(APawn* InteractingPawn);
};

