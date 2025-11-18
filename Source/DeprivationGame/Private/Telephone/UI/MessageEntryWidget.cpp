// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/UI/MessageEntryWidget.h"
#include "Kismet/KismetStringLibrary.h"

void UMessageEntryWidget::SetMessage(const FMessageData& Message)
{
    if (MessageText)
    {
        MessageText->SetText(FText::FromString(Message.Content));
    }
    else
    {
        // Если MessageText не привязан, выводим сообщение в лог
        UE_LOG(LogTemp, Warning, TEXT("MessageText widget not bound in MessageEntryWidget"));
    }
    
    // Обработка иконки отправителя
    if (SenderIcon)
    {
        // if (!Message.SenderIconPath.IsEmpty())
        // {
        // 	UTexture2D* IconTexture = LoadTextureFromPath(Message.SenderIconPath);
        // 	if (IconTexture)
        // 	{
        // 		SenderIcon->SetBrushFromTexture(IconTexture);
        // 	}
        // }
    }
    else
    {
        // Если SenderIcon не привязан, это не критично
        UE_LOG(LogTemp, Warning, TEXT("SenderIcon widget not bound in MessageEntryWidget (optional)"));
    }
}