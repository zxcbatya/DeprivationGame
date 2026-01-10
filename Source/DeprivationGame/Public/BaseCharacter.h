#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/EFatigueState.h"
#include "Enums/EDrunkState.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class ADeprivationCar;
class UMaterialParameterCollection;
class UUserWidget;
class UImage;
class UTextBlock;
class UInputAction;
class UInputMappingContext;

UCLASS()
class DEPRIVATIONGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, Category = "Tired")
	EFatigueState TiredState;

	UPROPERTY(VisibleAnywhere, Category = "Drunk")
	EDrunkState DrunkState;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialParameterCollection* StateMPC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "Camera Offset"))
	FVector CameraOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction",
		meta = (DisplayName = "Crosshair Widget Class"))
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction",
		meta = (DisplayName = "Interaction Widget Class"))
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayName = "Interaction Action"))
	UInputAction* InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayName = "Character Mapping Context"))
	UInputMappingContext* CharacterMappingContext;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Interaction", meta = (DisplayName = "Created Crosshair Widget"))
	UUserWidget* CreatedCrosshairWidget;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Interaction",
		meta = (DisplayName = "Created Interaction Widget"))
	UUserWidget* CreatedInteractionWidget;
	UPROPERTY()
	USceneComponent* ItemHoldSocket;
	UPROPERTY()
	class APickableItemActor* HoldItem;

	FTimerHandle InteractionCheckTimerHandle;
	FTimerHandle InteractDebounceTimerHandle;
	bool bCanInteract = true;

	AActor* CurrentHoveredInteractable = nullptr;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void CreateCrosshairWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void CreateInteractionWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowCrosshair(bool bShow);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowInteractionPrompt(bool bShow, const FText& PromptText = FText::GetEmpty());

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void StartChoppingMinigame();

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void EnterVehicle(APawn* Vehicle);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void ExitVehicle();

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void EnterVehicleByTag(FName VehicleTag);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void EnterVehicleByReference(ADeprivationCar* Vehicle);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* LineTrace(float LineLength, bool bDrawDebug = false) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* GetInteractableActor() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FText GetInteractionPrompt() const;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void CheckInteraction();
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PickUpItem(class APickableItemActor* ItemToPick);
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void DropItem();
	
	// Получить предмет, который держит персонаж
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	class APickableItemActor* GetHoldItem() const { return HoldItem; }
	
	// Установить предмет в руке (используется PlacementZone)
	void SetHoldItem(class APickableItemActor* NewItem) { HoldItem = NewItem; }
	
	// Проверить, находится ли персонаж в машине
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	bool IsInVehicle() const { return CurrentVehicle != nullptr; }
	
	// Получить текущую машину, в которой находится персонаж
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	ADeprivationCar* GetCurrentVehicle() const { return CurrentVehicle; }

private:
	UFUNCTION()
	void OnFatigueStateChanged(EFatigueState State);

	UFUNCTION()
	void OnDrunkStateChanged(EDrunkState State);

	ADeprivationCar* CurrentVehicle = nullptr;
};