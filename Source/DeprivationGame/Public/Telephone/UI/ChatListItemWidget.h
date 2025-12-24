// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/ChatData.h"
#include "ChatListItemWidget.generated.h"

UCLASS()
class DEPRIVATIONGAME_API UChatListItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UButton* ChatButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ContactNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LastMessageText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* UnreadCountText;

    UPROPERTY(meta = (BindWidget))
    UImage* UnreadIcon;

    UPROPERTY(meta = (BindWidget))
    UImage* ContactInitialIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ContactInitialText;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContactSelected, int32, ContactID);
    
    UPROPERTY(BlueprintAssignable, Category = "Chat")
    FOnContactSelected OnContactSelected;

private:
    int32 ContactID;
    FChatData ChatData;

protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void SetContactData(int32 InContactID, const FString& ContactName, const FString& LastMessage,
                                         int32 UnreadCount, const FLinearColor& ContactIconColor,
                                         const FString& ContactInitial);
    
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void HandleClicked();
};