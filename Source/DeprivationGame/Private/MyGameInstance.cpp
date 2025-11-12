// Fill out your copyright notice in the Description page of Project Settings.

#include "DeprivationGame/Public/MyGameInstance.h"
#include "DeprivationGame/Public/Telephone/ChatManagerSubsystem.h"

void UMyGameInstance::Init()
{
	Super::Init();

	// Initialize ChatManagerSubsystem
	UChatManagerSubsystem* ChatSubsystem = GetSubsystem<UChatManagerSubsystem>();
	if (ChatSubsystem)
	{
		// ChatManagerSubsystem is automatically initialized by UE
		// You can add any additional initialization here if needed
	}
}