// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "ChatListWidget.generated.h"

/**
 * Widget for displaying a list of chats
 */
UCLASS()
class DEPRIVATIONGAME_API UChatListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* ChatListContainer;

	UPROPERTY(EditAnywhere, Category = "Chat")
	TSubclassOf<class UChatListItemWidget> ChatItemClass;

	UPROPERTY(EditAnywhere, Category = "Chat")
	TSubclassOf<class UChatWindowWidget> ChatWindowClass;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Chat")
	void RefreshChatList(int32 ContactID = -1);

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void OpenChatWindow(int32 ContactID);

	// Обработчик клика по элементу списка
	UFUNCTION(BlueprintCallable, Category = "Chat")
	void OnChatItemClicked(int32 ContactID);


};
