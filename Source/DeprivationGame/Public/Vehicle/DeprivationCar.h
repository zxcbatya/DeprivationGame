#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "Interfaces/IInteractable.h"
#include "DeprivationCar.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogDeprivationCar, Log, All);

UCLASS()
class DEPRIVATIONGAME_API ADeprivationCar : public AWheeledVehiclePawn, public IInteractable
{
	GENERATED_BODY()

public:
	ADeprivationCar();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* VehicleMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AccelerateAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SteerAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* HandbrakeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* BrakeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ExitAction;

public:
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void EnterVehicle(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void ExitVehicle();

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Accelerate(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Steer(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Brake(const FInputActionValue& Value);

	virtual bool CanInteract_Implementation(APawn* InteractingPawn) const override;
	virtual void OnInteract_Implementation(APawn* InteractingPawn) override;
	virtual FText GetInteractionText_Implementation() const override;
	virtual float GetInteractionDistance_Implementation() const override;

private:
	void StopAccelerate();
	void StopSteer();
	void StopBrake();
	void HandbrakePressed();
	void HandbrakeReleased();
	void BindInputActions(UEnhancedInputComponent* EnhancedInput);

	APawn* CurrentDriver = nullptr;
	bool bCanExitVehicle = true;
	bool bIgnoreNextExit = false;
};
