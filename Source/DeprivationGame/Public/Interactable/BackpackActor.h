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
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sound")
	USoundBase* BackpackOpenSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sound")
	USoundBase* BackpackTakeItemSound;
public:
	UFUNCTION(BlueprintImplementableEvent, Category="Custom")
	void TakingVodka();
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
	void TakingDrugs();
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void InitializeBackpackContents();

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	int32 GetItemCount() const { return BackpackItems.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	AItemActor* GetNextItem();

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	bool HasMoreItems() const { return CurrentItemIndex < BackpackItems.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void SetPlacedAsContainer(bool bPlaced) { bIsPlacedAsContainer = bPlaced; }
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void RemoveItem(APawn* InteractingPawn);

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
