#pragma once

#include "CoreMinimal.h"
#include "Interactable/PickableItemActor.h"
#include "ItemActor.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	TOOTHBRUSHANDPASTE UMETA(DisplayName = "Toothbrush"),
	CLOTHES UMETA(DisplayName = "Clothes"),
	BACKPACK UMETA(DisplayName = "Backpack"),
	BOTTLE UMETA(DisplayName = "Bottle"),
	ZIPLOCK UMETA(DisplayName = "Ziplock"),
	NONE UMETA(DisplayName = "None")
};

UCLASS(Abstract, BlueprintType, Blueprintable)
class DEPRIVATIONGAME_API AItemActor : public APickableItemActor
{
	GENERATED_BODY()

public:
	AItemActor();

protected:
	// Тип предмета для фильтрации PlacementZone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;

	// Целевая зона размещения
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class APlacementZone> TargetPlacementZone;

public:
	// Получить тип предмета
	UFUNCTION(BlueprintCallable, Category = "Item")
	EItemType GetItemType() const { return ItemType; }

	// Установить тип предмета
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemType(EItemType NewItemType) { ItemType = NewItemType; }

	// Получить целевую зону размещения
	UFUNCTION(BlueprintCallable, Category = "Item")
	TSubclassOf<class APlacementZone> GetTargetPlacementZone() const { return TargetPlacementZone; }

	// Установить целевую зону размещения
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetTargetPlacementZone(TSubclassOf<class APlacementZone> NewZone) { TargetPlacementZone = NewZone; }

	// Событие использования предмета
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void OnItemUsed(APawn* User);
};