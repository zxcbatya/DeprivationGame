#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IInteractable.h"
#include "InteractableActor.generated.h"

class ABaseCharacter;
class APawn;
class UStaticMeshComponent;
class USceneComponent;

UCLASS(BlueprintType, Blueprintable)
class DEPRIVATIONGAME_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AInteractableActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract;

public:
	virtual bool CanInteract_Implementation(APawn* InteractingPawn) const override;
	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;
	virtual FText GetInteractionText_Implementation() const override;
	virtual float GetInteractionDistance_Implementation() const override;
	virtual void OnHoverBegin_Implementation(APawn* HoveringPawn) override;
	virtual void OnHoverEnd_Implementation(APawn* HoveringPawn) override;
	virtual void ShowInteractionPrompt_Implementation(APawn* InteractingPawn) override;
	virtual void HideInteractionPrompt_Implementation(APawn* InteractingPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void ReceiveOnInteract(APawn* InteractingPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	bool ReceiveCanInteract(APawn* InteractingPawn) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void ReceiveOnHoverBegin(APawn* HoveringPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void ReceiveOnHoverEnd(APawn* HoveringPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void ReceiveShowInteractionPrompt(APawn* InteractingPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void ReceiveHideInteractionPrompt(APawn* InteractingPawn);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetCanInteract(bool bNewCanInteract);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractionText(const FText& NewText);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractionDistance(float NewDistance);
};

