// Fill out your copyright notice in the Description page of Project Settings.


#include "DeprivationGame/Public/Telephone/UI/ChatListWidget.h"
#include "DeprivationGame/Public/Telephone/ChatManagerSubsystem.h"
#include "DeprivationGame/Public/Data/ChatData.h"
#include "Telephone/UI/MessageEntryWidget.h"

void UChatListWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>();
    if (Mgr)
    {
        Mgr->OnChatUpdated.AddDynamic(this, &UChatListWidget::RefreshChatList);
    }
    
    RefreshChatList(-1);
}


void UChatListWidget::RefreshChatList(int32 ContactID)
{
    UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>();
    if (!Mgr)
        return;
        
    //TArray<FMessageData> Chats = Mgr->GetAllChats();
    ChatListView->ClearListItems();
    
   //for (const auto& C : Chats)
     //  {
       //   UMessageEntryWidget* Item = CreateWidget<UMessageEntryWidget>(this, MessageEntryClass);
         //  Item->SetMessage(C);
           //ChatListView->AddItem(Item);
           
       //} 
}
