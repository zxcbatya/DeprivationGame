#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
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
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void CallVehicleBlueprintEnter();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void CallVehicleBlueprintExit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess="true"))
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* VehicleMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* CharacterMappingContext;

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
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LoockAction;

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

	/** Заблокировать управление машиной */
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void LockControls();

	/** Разблокировать управление машиной */
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void UnlockControls();

	/** Проверить, заблокировано ли управление */
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	bool IsControlsLocked() const { return bControlsLocked; }

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
	bool bControlsLocked = false;
protected:


	float CameraPitch;
	float CameraYaw;
	
	const float MaxPitchUp = 80.f;
	const float MaxPitchDown = -80.f;
	
	void Look(const FInputActionValue& Value);
	void UpdateCameraRotation() const;
	
};
