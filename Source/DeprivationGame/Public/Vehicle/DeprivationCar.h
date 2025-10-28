#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Components/BoxComponent.h"               
#include "InputActionValue.h"                     
#include "DeprivationCar.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogDeprivationCar, Log, All);

UCLASS()
class DEPRIVATIONGAME_API ADeprivationCar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ADeprivationCar();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* VehicleMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AccelerateAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* HandbrakeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SteerAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ExitAction;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Accelerate(const FInputActionValue& Value);

	void StopAccelerate();

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Steer(const FInputActionValue& Value);

	void StopSteer();

	void HandbrakePressed();
	void HandbrakeReleased();

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void EnterVehicle(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void ExitVehicle();

private:
	APawn* CurrentDriver = nullptr;
	FVector ActorPosition;

	UPROPERTY()
	TObjectPtr<UBoxComponent> TriggerBox; 

};
