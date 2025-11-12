// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/UI/ChatWindowWidget.h"
#include "Telephone/UI/MessageEntryWidget.h"
#include "Telephone/ChatManagerSubsystem.h"
#include "Components/VerticalBox.h"

void UChatWindowWidget::OpenChat(int32 ContactID)
{
	CurrentContactID = ContactID;
	RefreshMessages();
}

void UChatWindowWidget::RefreshMessages()
{
	UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>();
	if (!Mgr)
		return;

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