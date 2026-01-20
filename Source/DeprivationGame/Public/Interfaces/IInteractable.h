#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

UINTERFACE(BlueprintType)
class DEPRIVATIONGAME_API UInteractable : public UInterface
{
    GENERATED_BODY()
};

class DEPRIVATIONGAME_API IInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    bool CanInteract(APawn* InteractingPawn) const;
    virtual bool CanInteract_Implementation(APawn* InteractingPawn) const { return true; }

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    void OnInteract(APawn* InteractingPawn);
    virtual void OnInteract_Implementation(APawn* InteractingPawn) {}

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    FText GetInteractionText() const;
    virtual FText GetInteractionText_Implementation() const { return FText::GetEmpty(); }

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    float GetInteractionDistance() const;
    virtual float GetInteractionDistance_Implementation() const { return 100.0f; }
};