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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backpack")
	TArray<TSubclassOf<AItemActor>> BackpackContentsClasses;

	UPROPERTY()
	TArray<AItemActor*> BackpackItems;

	UPROPERTY(BlueprintReadOnly, Category = "Backpack")
	int32 CurrentItemIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backpack")
	bool bItemsLocked = false;
	UPROPERTY(BlueprintReadOnly, Category = "Backpack")
	bool bIsPlacedAsContainer = false;
	UPROPERTY(EditAnywhere, Category = "Backpack")
	UStaticMesh* BackpackMeshOpened;
	UPROPERTY(EditAnywhere, Category = "Backpack")
	UStaticMeshComponent* BackpackMesh;

public:
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void InitializeBackpackContents();

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	TArray<AItemActor*> GetBackpackContents() const { return BackpackItems; }

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	AItemActor* RemoveItemFromBackpack(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void AddItemToBackpack(AItemActor* Item);

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	bool IsValidPlacementSurface(class APlacementZone* Zone) const;

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	int32 GetItemCount() const { return BackpackItems.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	AItemActor* GetNextItem();

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	bool HasMoreItems() const { return CurrentItemIndex < BackpackItems.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void SetItemsLocked(bool bLocked) { bItemsLocked = bLocked; }

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	bool IsItemsLocked() const { return bItemsLocked; }

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void ResetItemIndex() { CurrentItemIndex = 0; }

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void SetPlacedAsContainer(bool bPlaced) { bIsPlacedAsContainer = bPlaced; }

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	bool IsPlacedAsContainer() const { return bIsPlacedAsContainer; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Backpack")
	void OnBackpackOpened(APawn* InteractingPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "Backpack")
	void OnBackpackClosed(APawn* InteractingPawn);

	virtual bool CanInteract_Implementation(APawn* InteractingPawn) const override;

	void ProcessBackpackInteraction(APawn* InteractingPawn);

	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Backpack")
	void OnItemRemovedFromBackpack(AItemActor* Item, APawn* InteractingPawn);

	void HandleBackpackInteraction(APawn* InteractingPawn);

private:
	bool bIsOpened = false;
	bool blsHoldItem = false;
};
