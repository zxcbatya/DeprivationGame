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
	// Содержимое рюкзака в порядке извлечения
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backpack")
	TArray<TSubclassOf<AItemActor>> BackpackContentsClasses;

	// Актуальные экземпляры предметов в рюкзаке
	UPROPERTY()
	TArray<AItemActor*> BackpackItems;

	// Индекс следующего предмета для извлечения
	UPROPERTY(BlueprintReadOnly, Category = "Backpack")
	int32 CurrentItemIndex = 0;

	// Флаг блокировки извлечения предметов
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backpack")
	bool bItemsLocked = false;

	// Флаг, показывающий что рюкзак уже был размещён и теперь используется как контейнер
	UPROPERTY(BlueprintReadOnly, Category = "Backpack")
	bool bIsPlacedAsContainer = false;

public:
	// Инициализировать содержимое рюкзака
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void InitializeBackpackContents();

	// Получить содержимое рюкзака
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	TArray<AItemActor*> GetBackpackContents() const { return BackpackItems; }

	// Достать предмет из рюкзака
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	AItemActor* RemoveItemFromBackpack(int32 Index);

	// Добавить предмет в рюкзак
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void AddItemToBackpack(AItemActor* Item);

	// Проверка валидности поверхности для размещения
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	bool IsValidPlacementSurface(class APlacementZone* Zone) const;

	// Получить количество предметов в рюкзаке
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	int32 GetItemCount() const { return BackpackItems.Num(); }

	// Получить следующий предмет по порядку
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	AItemActor* GetNextItem();

	// Проверить, есть ли еще предметы
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	bool HasMoreItems() const { return CurrentItemIndex < BackpackItems.Num(); }

	// Блокировка/разблокировка извлечения предметов
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void SetItemsLocked(bool bLocked) { bItemsLocked = bLocked; }

	// Проверить, заблокировано ли извлечение
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	bool IsItemsLocked() const { return bItemsLocked; }

	// Сбросить индекс к началу
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void ResetItemIndex() { CurrentItemIndex = 0; }

	// Установить флаг контейнера
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void SetPlacedAsContainer(bool bPlaced) { bIsPlacedAsContainer = bPlaced; }

	// Получить флаг контейнера
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	bool IsPlacedAsContainer() const { return bIsPlacedAsContainer; }

	// Событие открытия рюкзака
	UFUNCTION(BlueprintImplementableEvent, Category = "Backpack")
	void OnBackpackOpened(APawn* InteractingPawn);

	// Событие закрытия рюкзака
	UFUNCTION(BlueprintImplementableEvent, Category = "Backpack")
	void OnBackpackClosed(APawn* InteractingPawn);

	// Переопределение CanInteract для контейнерной логики
	virtual bool CanInteract_Implementation(APawn* InteractingPawn) const override;

	// Обработка взаимодействия с рюкзаком
	void ProcessBackpackInteraction(APawn* InteractingPawn);

	// Переопределение OnInteract - основная логика
	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;

	// Событие извлечения предмета из рюкзака
	UFUNCTION(BlueprintImplementableEvent, Category = "Backpack")
	void OnItemRemovedFromBackpack(AItemActor* Item, APawn* InteractingPawn);
	
	// Обработка взаимодействия с рюкзаком (основная логика)
	void HandleBackpackInteraction(APawn* InteractingPawn);
};