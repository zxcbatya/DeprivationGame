#pragma once

#include "CoreMinimal.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "ChaosVehicles/Public/WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "DeprivationCar.generated.h"

UCLASS()
class DEPRIVATIONGAME_API ADeprivationCar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ADeprivationCar();

	UPROPERTY(VisibleAnywhere, Category="Vehicle")
	class UBoxComponent* EnterTrigger;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* VehicleMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* AccelerateAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* BrakeAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* SteerAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ExitAction;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							   const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Accelerate(const FInputActionValue& Value);
	void StopAccelerate();
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Brake(const FInputActionValue& Value);
	void StopBrake();
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Steer(const FInputActionValue& Value);
	void StopSteer();
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void EnterVehicle(APawn* Pawn);
	
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void ExitVehicle();
	
private:
	APawn* CurrentDriver = nullptr;
	FVector ActorPosition;
};