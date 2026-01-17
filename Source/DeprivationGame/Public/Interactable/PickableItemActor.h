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

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FVector OriginalLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FRotator OriginalRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	bool bIsHeld = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand Attachment")
	FVector HandOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand Attachment")
	FRotator HandRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand Attachment")
	FName HandSocketName = "hand_r_socket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	TArray<TSubclassOf<class APlacementZone>> ValidPlacementZones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bCannotBePickedUpAgain = false;

public:
	virtual bool CanInteract_Implementation(APawn* InteractingPawn) const override;
	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickable Item")
	void ReceiveItemPicked(APawn* InteractingPawn);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void ReturnToOriginalPosition();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SaveOriginalTransform();

	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsHeld() const { return bIsHeld; }

	void SetIsHeld(bool bNewIsHeld) { bIsHeld = bNewIsHeld; }

	UFUNCTION(BlueprintCallable, Category = "Hand Attachment")
	FVector GetHandOffset() const { return HandOffset; }

	UFUNCTION(BlueprintCallable, Category = "Hand Attachment")
	FRotator GetHandRotation() const { return HandRotation; }

	UFUNCTION(BlueprintCallable, Category = "Hand Attachment")
	FName GetHandSocketName() const { return HandSocketName; }

	UFUNCTION(BlueprintCallable, Category = "Hand Attachment")
	void SetHandOffset(const FVector& NewOffset) { HandOffset = NewOffset; }

	UFUNCTION(BlueprintCallable, Category = "Hand Attachment")
	void SetHandRotation(const FRotator& NewRotation) { HandRotation = NewRotation; }

	UFUNCTION(BlueprintCallable, Category = "Hand Attachment")
	void SetHandSocketName(const FName& NewSocketName) { HandSocketName = NewSocketName; }

	// Проверка валидности зоны размещения
	UFUNCTION(BlueprintCallable, Category = "Placement")
	bool IsValidPlacementZone(class APlacementZone* Zone) const;

	// Установить флаг невозможности повторного подбора
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetCannotBePickedUpAgain(bool bCannotPickup) { bCannotBePickedUpAgain = bCannotPickup; }

	// Проверить, можно ли подобрать предмет
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanBePickedUp() const { return !bCannotBePickedUpAgain; }
};

