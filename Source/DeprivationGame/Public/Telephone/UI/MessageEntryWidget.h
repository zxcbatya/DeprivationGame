// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeprivationGame/Public/Data/ChatData.h"
#include "MessageEntryWidget.generated.h"

/**
 * Widget for displaying a single chat message
 */
UCLASS()
class DEPRIVATIONGAME_API UMessageEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMessage(const FMessageData& Message);
};