// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBPI_VehicleInteract.generated.h"

// This class does not need to be modified.

UINTERFACE(MinimalAPI)
class UVehicleInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class DEPRIVATIONGAME_API IVehicleInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Vehicle")
	void EnterVehicle(class ADeprivationCar* Vehicle);
    
	UFUNCTION(BlueprintNativeEvent, Category = "Vehicle")
	void ExitVehicle();
};