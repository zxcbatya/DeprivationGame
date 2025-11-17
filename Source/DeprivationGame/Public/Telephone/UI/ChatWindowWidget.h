// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Telephone/ChatManagerSubsystem.h"
#include "Data/ChatData.h"
#include "ChatListWidget.h"
#include "ChatWindowWidget.generated.h"

/**
 * Widget for displaying chat messages with a specific contact
 */
UCLASS()
class DEPRIVATIONGAME_API UChatWindowWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:
    UPROPERTY(meta=(BindWidget))
    UVerticalBox* MessagesBox;

    UPROPERTY(meta=(BindWidget))
    UButton* BackButton;

    UPROPERTY(EditAnywhere, Category = "Chat")
    TSubclassOf<class UChatListWidget> ChatListClass;

    UPROPERTY(EditAnywhere, Category = "Chat")
    TSubclassOf<class UMessageEntryWidget> MessageEntryClass;

private:
    int32 CurrentContactID;
    UChatListWidget* ParentChatList;

public:
    UFUNCTION(BlueprintCallable, Category = "Chat")
    void OpenChat(int32 ContactID, UChatListWidget* ParentList);

    UFUNCTION(BlueprintCallable, Category = "Chat")
    void RefreshMessages();

private:
    UFUNCTION()
    void OnBackClicked();
};