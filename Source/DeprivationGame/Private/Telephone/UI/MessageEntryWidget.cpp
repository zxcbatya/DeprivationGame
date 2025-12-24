// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/UI/MessageEntryWidget.h"

void UMessageEntryWidget::SetMessage(const FMessageData& Message)
{
    if (MessageText)
    {
        MessageText->SetText(FText::FromString(Message.Content));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MessageText widget not bound in MessageEntryWidget"));
    }
    
    if (SenderIcon)
    {
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SenderIcon widget not bound in MessageEntryWidget (optional)"));
    }
}