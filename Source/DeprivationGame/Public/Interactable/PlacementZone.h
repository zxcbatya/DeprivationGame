#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "PlacementZone.generated.h"

class APickableItemActor;

UCLASS(BlueprintType, Blueprintable)
class DEPRIVATIONGAME_API APlacementZone : public AInteractableActor
{
	GENERATED_BODY()

public:
	APlacementZone();

protected:
	virtual void BeginPlay() override;

	// Текущий предмет, размещенный в зоне (если есть)
	UPROPERTY(BlueprintReadOnly, Category = "Placement")
	APickableItemActor* PlacedItem;

	// Сокет для размещения предмета (опционально)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* PlacementSocket;

	// Можно ли разместить предмет в этой зоне
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	bool bCanPlaceItem = true;

	// Разрешен ли только один предмет за раз
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	bool bSingleItemOnly = true;

public:
	virtual bool CanInteract_Implementation(APawn* InteractingPawn) const override;
	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;

	// Размещает предмет в зоне
	UFUNCTION(BlueprintCallable, Category = "Placement")
	bool PlaceItem(APickableItemActor* ItemToPlace);

	// Убирает предмет из зоны (если есть)
	UFUNCTION(BlueprintCallable, Category = "Placement")
	void RemoveItem();

	// Проверяет, можно ли разместить предмет в этой зоне
	UFUNCTION(BlueprintCallable, Category = "Placement")
	bool CanPlaceItem(APickableItemActor* ItemToPlace) const;

	// Получить текущий размещенный предмет
	UFUNCTION(BlueprintCallable, Category = "Placement")
	APickableItemActor* GetPlacedItem() const { return PlacedItem; }

	// Переопределяем GetInteractionText чтобы показывать правильный текст в зависимости от наличия предмета в руке
	virtual FText GetInteractionText_Implementation() const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Placement")
	void OnItemPlaced(APickableItemActor* ItemPlaced);

	UFUNCTION(BlueprintImplementableEvent, Category = "Placement")
	void OnItemRemoved(APickableItemActor* ItemRemoved);
};

