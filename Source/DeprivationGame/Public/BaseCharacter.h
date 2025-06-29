#pragma once

#include "CoreMinimal.h"
#include "Enums/EDrunkState.h"
#include "Enums/EFatigueState.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

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

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void OnFatigueStateChanged(EFatigueState State);

	UFUNCTION()
	void OnDrunkStateChanged(EDrunkState State);
};
