// BaseCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerComponent/StateComponent.h"
#include "PlayerComponent/InteractableComponent.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class ADeprivationCar;
class UMaterialParameterCollection;
class UUserWidget;
class UImage;
class UTextBlock;
class UInputAction;
class UInputMappingContext;
class APickableItemActor;

UCLASS()
class DEPRIVATIONGAME_API ABaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABaseCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    UCharacterStateComponent* StateComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    UInteractableComponent* InteractableComponent;
    
public:
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    UInteractableComponent* GetInteractableComponent() const { return InteractableComponent; }
    
    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialParameterCollection* StateMPC;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComponent;

public:
    UFUNCTION(BlueprintCallable, Category = "Camera")
    UCameraComponent* GetCameraComponent() const { return CameraComponent; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayName = "Interaction Action"))
    UInputAction* InteractionAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayName = "Character Mapping Context"))
    UInputMappingContext* CharacterMappingContext;

    FTimerHandle InteractionCheckTimerHandle;
    FTimerHandle InteractDebounceTimerHandle;
    bool bCanInteract = true;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void StartChoppingMinigame();
    
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void EnterVehicle(APawn* Vehicle);
    
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void ExitVehicle();
    
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void EnterVehicleByTag(FName VehicleTag);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    AActor* LineTrace(float LineLength, bool bDrawDebug = false) const;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Interact();
    
    // Обработчики делегатов взаимодействия
    UFUNCTION()
    void HandleInteractionStarted(AActor* InteractableActor, ABaseCharacter* Character);
    
    UFUNCTION()
    void HandleInteractionCompleted(AActor* InteractableActor, ABaseCharacter* Character);
    
    UFUNCTION()
    void HandleInteractionCancelled(AActor* InteractableActor, ABaseCharacter* Character);

    UPROPERTY(Transient)
    USceneComponent* ItemHoldSocket;
    
    UPROPERTY(Transient)
    APickableItemActor* blsHoldItem;

    friend class UInteractableComponent;
    ADeprivationCar* CurrentVehicle = nullptr;
    AActor* CurrentHoveredInteractable = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void PickUpItem(APickableItemActor* ItemToPick);
    
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    APickableItemActor* GetHoldItem() const { return blsHoldItem; }
    
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetHoldItem(APickableItemActor* NewItem) { blsHoldItem = NewItem; }
    
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    bool IsInVehicle() const { return CurrentVehicle != nullptr; }
    
};