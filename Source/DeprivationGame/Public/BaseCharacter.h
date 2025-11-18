#pragma once

#include "CoreMinimal.h"
#include "Enums/EDrunkState.h"
#include "Enums/EFatigueState.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "BaseCharacter.generated.h"

class ADeprivationCar;

UCLASS()
class DEPRIVATIONGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;
	bool bIsTired;
	UPROPERTY(VisibleAnywhere, Category="Tireds")
	USoundBase* TiredSound;
	UPROPERTY(VisibleAnywhere, Category="Tireds")
	EFatigueState TiredState;
	UPROPERTY(VisibleAnywhere, Category="Drunk")
	EDrunkState DrunkState;

	UFUNCTION(BlueprintCallable, Category="Player State")
	void SetFatigueState(EFatigueState NewState);

	UFUNCTION(BlueprintCallable, Category="Player State")
	void SetDrunkState(EDrunkState NewState);

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialParameterCollection* StateMPC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	// Редактируемое смещение камеры в редакторе
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "Camera Offset"))
	FVector CameraOffset;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void EnterVehicle(APawn* Vehicle);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void ExitVehicle();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* LineTrace(float LineLength, bool bDrawDebug = false) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* GetInteractableActor() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FText GetInteractionPrompt() const;

private:
	UFUNCTION()
	void OnFatigueStateChanged(EFatigueState State);

	UFUNCTION()
	void OnDrunkStateChanged(EDrunkState State);

	ADeprivationCar* CurrentVehicle = nullptr;
};