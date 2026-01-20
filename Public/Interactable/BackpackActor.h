#pragma once

#include "CoreMinimal.h"
#include "Interactable/ItemActor.h"
#include "BackpackActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class DEPRIVATIONGAME_API ABackpackActor : public AItemActor
{
	GENERATED_BODY()

public:
	ABackpackActor();

protected:
	// Mesh компоненты
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BackpackMesh;

	// Открытая сетка рюкзака
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMesh* BackpackMeshOpened;

	// Содержимое рюкзака
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<TSubclassOf<AItemActor>> BackpackContentsClasses;

	// Предметы в рюкзаке
	UPROPERTY()
	TArray<AItemActor*> BackpackItems;

	// Текущий индекс для извлечения
	UPROPERTY()
	int32 CurrentItemIndex;

	// Может ли быть размещён как контейнер
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement")
	bool bCanBePlacedAsContainer;

	// Размещён ли как контейнер
	UPROPERTY()
	bool bIsPlacedAsContainer;

	// Заблокированы ли предметы
	UPROPERTY()
	bool bItemsLocked;

	// Валидные зоны размещения
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement")
	TArray<TSubclassOf<class APlacementZone>> ValidPlacementZones;

public:
	// Blueprint-callable методы для управления предметами
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasMoreItems() const { return CurrentItemIndex < BackpackItems.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemCount() const { return BackpackItems.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetRemainingItemCount() const { return BackpackItems.Num() - CurrentItemIndex; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItemActor* GetItemAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItemActor* GetNextItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToBackpack(AItemActor* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItemActor* RemoveItemFromBackpack(int32 Index);

	// Методы управления состоянием
	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsOpened() const { return bIsOpened; }

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetOpened(bool bNewOpened) { bIsOpened = bNewOpened; }

	// Методы размещения
	UFUNCTION(BlueprintCallable, Category = "Placement")
	bool IsValidPlacementSurface(class APlacementZone* Zone) const;

	// События для Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnItemRemovedFromBackpack(AItemActor* Item, APawn* InteractingPawn);

protected:
	virtual bool CanInteract_Implementation(APawn* InteractingPawn) const override;
	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;

private:
	bool bIsOpened = false;
	bool blsHoldItem = false;

	void HandleBackpackInteraction(APawn* InteractingPawn);
	void InitializeBackpackContents();
};