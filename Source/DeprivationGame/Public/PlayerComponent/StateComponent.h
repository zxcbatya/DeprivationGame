#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Normal,
	FatiguedStage1,
	FatiguedStage2,
	FatiguedStage3,
	DrunkStage1,
	DrunkStage2,
	DrunkStage3,
	InputLocked,
	CinematicPlaying,
	HoldItem,
	InVehicle
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStateChanged, ECharacterState, NewState);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEPRIVATIONGAME_API UCharacterStateComponent : public UActorComponent
{
	GENERATED_BODY()
 
public:
	UCharacterStateComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetState(ECharacterState NewState);
	
	UFUNCTION(BlueprintPure, Category = "State")
	ECharacterState GetCurrentState() const;

	FOnCharacterStateChanged OnCharacterStateChanged;
 
private:
	UPROPERTY(VisibleAnywhere, Category = "State")
	ECharacterState CurrentState;

	
};