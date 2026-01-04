#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "DoorActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class DEPRIVATIONGAME_API ADoorActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	ADoorActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void ReceiveDoorInteract(APawn* InteractingPawn);
};

