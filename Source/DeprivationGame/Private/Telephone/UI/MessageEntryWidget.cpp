// Fill out your copyright notice in the Description page of Project Settings.

#include "DeprivationGame/Public/Telephone/UI/MessageEntryWidget.h"
#include "DeprivationGame/Public/Data/ChatData.h"

void UMessageEntryWidget::SetMessage(const FMessageData& Message) const
{
	 if (MessageTextBlock)
	 {
	     MessageTextBlock->SetText(FText::FromString(Message.Content));
	 }
}
