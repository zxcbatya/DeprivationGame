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

	// Исходная позиция и вращение предмета для возврата на место
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FVector OriginalLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FRotator OriginalRotation;

	// Флаг, показывающий, что предмет был взят в руку
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	bool bIsHeld = false;

public:
	virtual bool CanInteract_Implementation(APawn* InteractingPawn) const override;
	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickable Item")
	void ReceiveItemPicked(APawn* InteractingPawn);

	// Возвращает предмет на исходную позицию
	UFUNCTION(BlueprintCallable, Category = "Item")
	void ReturnToOriginalPosition();

	// Сохраняет текущую позицию как исходную
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SaveOriginalTransform();

	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsHeld() const { return bIsHeld; }

	void SetIsHeld(bool bNewIsHeld) { bIsHeld = bNewIsHeld; }
};

