// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/UI/ChatListWidget.h"
#include "Telephone/ChatManagerSubsystem.h"
#include "Data/ChatData.h"
#include "Components/VerticalBox.h"
#include "Telephone/UI/ChatWindowWidget.h"
#include "Telephone/UI/ChatListItemWidget.h"
#include "Kismet/GameplayStatics.h"

void UChatListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Подписываемся на обновления чатов
	if (UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>())
	{
		Mgr->OnChatUpdated.AddDynamic(this, &UChatListWidget::RefreshChatList);
	}

	RefreshChatList();
}

void UChatListWidget::NativeDestruct()
{
	// Отписываемся от обновлений чатов
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

	UE_LOG(LogTemp, Warning, TEXT("Refreshing chat list, found %d chats"), Chats.Num());

	// Создаем элементы списка для каждого чата
	for (const FChatData& Chat : Chats)
	{
		if (ChatItemClass)
		{
			UChatListItemWidget* ChatItem = CreateWidget<UChatListItemWidget>(this, ChatItemClass);
			if (ChatItem)
			{
				// Настраиваем элемент списка с данными чата
				FString LastMessage = Chat.Messages.Num() > 0 ? Chat.Messages.Last().Content : TEXT("No messages");
				ChatItem->SetContactData(
					Chat.ChatId,
					Chat.ContactName,
					LastMessage,
					Chat.UnreadCount,
					Chat.ContactIconColor,
					Chat.ContactInitial

				);

				// Подписываемся на клик по элементу
				ChatItem->OnContactSelected.AddDynamic(this, &UChatListWidget::OnChatItemClicked);

				ChatListContainer->AddChild(ChatItem);
			}
		}
	}
}

void UChatListWidget::OnChatItemClicked(int32 ContactID)
{
	OpenChatWindow(ContactID);
}


void UChatListWidget::OpenChatWindow(int32 ContactID)
{
	if (!ChatWindowClass) return;

	UChatWindowWidget* ChatWindow = CreateWidget<UChatWindowWidget>(GetWorld(), ChatWindowClass);
	if (ChatWindow)
	{
		ChatWindow->OpenChat(ContactID, this); 

		ChatWindow->AddToViewport();
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}
