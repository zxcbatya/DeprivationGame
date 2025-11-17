// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ChatData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MessageEntryWidget.generated.h"

/**
 * Widget for displaying a single chat message
 */
UCLASS()
class DEPRIVATIONGAME_API UMessageEntryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category="Settings")
    UTextBlock* MessageText;

    UPROPERTY(VisibleAnywhere, Category="Settings")
    UImage* SenderIcon;
    
    UPROPERTY(VisibleAnywhere, Category="Settings")
    UImage* PanelIcon;

    UFUNCTION(BlueprintCallable, Category = "Message")
    void SetMessage(const FMessageData& Message);
};