// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/UI/ChatListWidget.h"
#include "Telephone/ChatManagerSubsystem.h"
#include "Data/ChatData.h"
#include "Components/VerticalBox.h"
#include "Telephone/UI/ChatWindowWidget.h"
#include "Telephone/UI/ChatListItemWidget.h"

void UChatListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>())
	{
		Mgr->OnChatUpdated.AddDynamic(this, &UChatListWidget::RefreshChatList);
	}

	RefreshChatList();
}

void UChatListWidget::NativeDestruct()
{
	if (UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>())
	{
		Mgr->OnChatUpdated.RemoveDynamic(this, &UChatListWidget::RefreshChatList);
	}

	Super::NativeDestruct();
}

void UChatListWidget::RefreshChatList(int32 ContactID)
{
	UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>();
	if (!Mgr || !ChatListContainer)
		return;

	TArray<FChatData> Chats = Mgr->GetAllChats();
	ChatListContainer->ClearChildren();


	for (const FChatData& Chat : Chats)
	{
		UChatListItemWidget* ChatItem = CreateWidget<UChatListItemWidget>(this, ChatItemClass);
		FString LastMessage = Chat.Messages.Num() > 0 ? Chat.Messages.Last().Content : TEXT("No messages");
		ChatItem->SetContactData(
			Chat.ChatId,
			Chat.ContactName,
			LastMessage,
			Chat.UnreadCount,
			Chat.ContactIconColor,
			Chat.ContactInitial

		);

		ChatItem->OnContactSelected.AddDynamic(this, &UChatListWidget::OnChatItemClicked);

		ChatListContainer->AddChild(ChatItem);
	}
}

void UChatListWidget::OnChatItemClicked(int32 ContactID)
{
	OpenChatWindow(ContactID);
}


void UChatListWidget::OpenChatWindow(int32 ContactID)
{
	UChatWindowWidget* ChatWindow = CreateWidget<UChatWindowWidget>(GetWorld(), ChatWindowClass);
	if (ChatWindow)
	{
		ChatWindow->OpenChat(ContactID, this);

		ChatWindow->AddToViewport();
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}
