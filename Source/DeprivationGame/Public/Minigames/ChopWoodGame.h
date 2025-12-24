#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "ChopWoodGame.generated.h"

class UChopWoodWidget;
class UInputMappingContext;
class UInputAction;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEPRIVATIONGAME_API UChopWoodGame : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float TargetZoneSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float IndicatorSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "ChopWood")
	bool IsChopping;

	UPROPERTY(BlueprintReadWrite, Category = "ChopWood")
	float IndicatorPosition;

	UPROPERTY(BlueprintReadWrite, Category = "ChopWood")
	bool IndicatorDirection;

	UPROPERTY(BlueprintReadOnly, Category = "ChopWood")
	int32 ChopsSuccessful;

	UPROPERTY(BlueprintReadOnly, Category = "ChopWood")
	int32 ChopsFailed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood", meta = (ClampMin = "1", ClampMax = "10"))
	int32 RequiredHits;

	UPROPERTY(BlueprintReadWrite, Category = "ChopWood")
	float TargetZoneCenter;

	UPROPERTY(BlueprintReadWrite, Category = "ChopWood")
	float CurrentTargetZoneSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float TargetZoneShrinkStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float MinTargetZoneSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float InitialTargetZoneSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float InitialTargetZoneCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	TSubclassOf<UChopWoodWidget> ChopWoodWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* ChopWoodMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ChopAction;

	UPROPERTY()
	bool bWasRotationInputAllowed;

	UPROPERTY()
	bool bWasClickEventsAllowed;

	UPROPERTY()
	bool bWasMouseOverEventsAllowed;

	UPROPERTY()
	bool bWasLookInputAllowed;

	UPROPERTY()
	bool bWasMoveInputAllowed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Timing")
	float WidgetDisplayDelay;

protected:
	FTimerHandle WidgetDisplayTimerHandle;

public:
	UChopWoodGame();

	UFUNCTION(BlueprintCallable, Category = "ChopWood")
	void StartChopping();

	UFUNCTION(BlueprintCallable, Category = "ChopWood")
	void StopChopping();

	UFUNCTION(BlueprintCallable, Category = "ChopWood")
	bool AttemptChop();

	UFUNCTION(BlueprintCallable, Category = "ChopWood")
	void StartMinigameFromActor(AActor* InteractingActor);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	virtual void BeginPlay() override;

	void HandleChopInput(const FInputActionValue& Value);

protected:
	void InitializeDefaults();
	
	TWeakObjectPtr<UChopWoodWidget> CreatedWidget;

private:
	void BindInputActions(UInputComponent* PlayerInputComponent);
	void UpdateWidget();
	void NotifyGameCompleted(bool bSuccess);
	void NotifyHitResult(bool bSuccess);
	
	void SavePlayerInputState(APlayerController* PC);
	void RestorePlayerInputState(APlayerController* PC);
	void DisableAllPlayerInput(APlayerController* PC);
	void EnableAllPlayerInput(APlayerController* PC);
};