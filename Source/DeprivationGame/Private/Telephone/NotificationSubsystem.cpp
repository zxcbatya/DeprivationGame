// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/NotificationSubsystem.h"

void UNotificationSubsystem::ShowNotification(int32 ContactID, const FMessageData& Message)
{
    OnNotificationReceived.Broadcast(ContactID, Message);
}