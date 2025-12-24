// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "DeprivationGame/Public/Telephone/ChatManagerSubsystem.h"
#include "DeprivationGame/Public/Data/ChatData.h"
#include "ChatWindowWidget.generated.h"

/**
 * Widget for displaying chat messages with a specific contact
 */
UCLASS()
class DEPRIVATIONGAME_API UChatWindowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* MessagesBox;

	UPROPERTY(EditAnywhere, Category = "Chat")
	TSubclassOf<class UMessageEntryWidget> MessageEntryClass;

private:
	int32 CurrentContactID;

public:
	void OpenChat(int32 ContactID);
	void RefreshMessages();
};