// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/UI/ChatListItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UChatListItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ChatButton->OnClicked.AddDynamic(this, &UChatListItemWidget::HandleClicked);
}

void UChatListItemWidget::SetContactData(int32 InContactID, const FString& ContactName, const FString& LastMessage,
                                         int32 UnreadCount, const FLinearColor& ContactIconColor,
                                         const FString& ContactInitial)
{
	ContactID = InContactID;
	ContactNameText->SetText(FText::FromString(ContactName));
	LastMessageText->SetText(FText::FromString(LastMessage));
	UnreadCountText->SetText(FText::AsNumber(UnreadCount));
	const ESlateVisibility NewVisibility = (UnreadCount > 0) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	UnreadCountText->SetVisibility(NewVisibility);
	UnreadIcon->SetVisibility(NewVisibility);
	ContactInitialIcon->SetColorAndOpacity(ContactIconColor);
	ContactInitialText->SetText(FText::FromString(ContactInitial));
}

void UChatListItemWidget::HandleClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Chat item clicked, ContactID: %d"), ContactID);
	OnContactSelected.Broadcast(ContactID);
}
