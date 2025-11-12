// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Telephone/ChatManagerSubsystem.h"
#include "Data/ChatData.h"
#include "ChatListWidget.generated.h"

/**
 * Widget for displaying a list of chats
 */
UCLASS()
class DEPRIVATIONGAME_API UChatListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UListView* ChatListView;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void RefreshChatList(int32 ContactID);
};