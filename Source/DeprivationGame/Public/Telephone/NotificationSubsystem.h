// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/ChatData.h"
#include "NotificationSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNotificationReceived, int32, ContactID, FMessageData, Message);

UCLASS()
class DEPRIVATIONGAME_API UNotificationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnNotificationReceived OnNotificationReceived;

    UFUNCTION(BlueprintCallable, Category = "Notification")
    void ShowNotification(int32 ContactID, const FMessageData& Message);
};