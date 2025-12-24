// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/UI/ChatWindowWidget.h"
#include "Telephone/UI/MessageEntryWidget.h"
#include "Telephone/UI/ChatListWidget.h"
#include "Telephone/ChatManagerSubsystem.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

void UChatWindowWidget::OpenChat(int32 ContactID, UChatListWidget* ParentList)
{
	CurrentContactID = ContactID;
	ParentChatList = ParentList;
	RefreshMessages();
}

void UChatWindowWidget::RefreshMessages()
{
	UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>();

	FChatData Chat = Mgr->GetChatByContactID(CurrentContactID);
	MessagesBox->ClearChildren();

	for (const auto& Msg : Chat.Messages)
	{
		if (MessageEntryClass)
		{
			UMessageEntryWidget* Entry = CreateWidget<UMessageEntryWidget>(this, MessageEntryClass);
			if (Entry)
			{
				Entry->SetMessage(Msg);
				MessagesBox->AddChild(Entry);
			}
		}
	}

	Mgr->MarkChatAsRead(CurrentContactID);
}

void UChatWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UChatWindowWidget::OnBackClicked);
	}
}

void UChatWindowWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (BackButton) BackButton->OnClicked.RemoveDynamic(this, &UChatWindowWidget::OnBackClicked);
}

void UChatWindowWidget::OnBackClicked()
{
	UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>();

	if (UChatManagerSubsystem* ChatManagerSubsystem = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>())
	{
		ChatManagerSubsystem->RequestChatListRefresh(CurrentContactID);
	}
	if (ParentChatList)
	{
		ParentChatList->SetVisibility(ESlateVisibility::Visible);
		ParentChatList->RefreshChatList(CurrentContactID); // ← Принудительно обновляем
	}
	SetVisibility(ESlateVisibility::Hidden);
	RemoveFromParent();
}
