#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "Axe.generated.h"

UCLASS()
class DEPRIVATIONGAME_API AAxe : public AInteractableActor
{
	GENERATED_BODY()

public:
	AAxe();

	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;
	virtual bool CanInteract_Implementation(APawn* InteractingPawn) const override;

protected:
	virtual void BeginPlay() override;
};