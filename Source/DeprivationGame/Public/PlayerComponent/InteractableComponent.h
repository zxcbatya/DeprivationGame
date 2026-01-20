// InteractableComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IInteractable.h"
#include "Delegates/Delegate.h"
#include "InteractableComponent.generated.h"

class ABaseCharacter;
class APickableItemActor;
class ADeprivationCar;
class UUserWidget;
class USceneComponent;

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	Container,
	Pickable,
	Standard,
	LongerInteraction
};

// Делегат для события взаимодействия
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionEvent, AActor*, InteractableActor, ABaseCharacter*,
                                             Character);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEPRIVATIONGAME_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	TSubclassOf<UUserWidget> InteractWidgetClass;
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

public:
	UInteractableComponent();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* GetInteractableActor() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void CheckInteraction();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PickUpItem(APickableItemActor* ItemToPick);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowCrosshair(bool bShow);
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* LineTrace(float LineLength, bool bDrawDebug = false) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	APickableItemActor* GetHoldItem() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetHoldItem(APickableItemActor* NewItem);
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void DropItem();

	// Делегаты событий взаимодействия
	UPROPERTY(BlueprintAssignable, Category = "Interaction Events")
	FOnInteractionEvent OnInteractionStarted;

	UPROPERTY(BlueprintAssignable, Category = "Interaction Events")
	FOnInteractionEvent OnInteractionCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Interaction Events")
	FOnInteractionEvent OnInteractionCancelled;

private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void CreateWidgets();

	UPROPERTY()
	UUserWidget* CreatedCrosshairWidget;
	ABaseCharacter* CharacterOwner;
	UPROPERTY()
	USceneComponent* ItemHoldSocket;
	UPROPERTY()
	APickableItemActor* HeldItem;

	FTimerHandle InteractionCheckTimerHandle;
	FTimerHandle InteractDebounceTimerHandle;

	bool bCanInteract;
	bool bIsCrosshairVisible;
	TWeakObjectPtr<AActor> CurrentHoveredInteractable;
	float LastLineTraceTime;


	static EInteractionType DetermineInteractionType(AActor* Interactable);

	// Методы для делегатной системы взаимодействия
	void ProcessInteractionByType(AActor* Interactable, EInteractionType Type);
	void HandleContainerInteraction(AActor* Container);
	void HandlePickableInteraction(AActor* Pickable);
	void HandleStandardInteraction(AActor* Interactable);
};
