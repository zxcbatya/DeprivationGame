// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Telephone/ChatManagerSubsystem.h"
#include "ChatExample.generated.h"

UCLASS()
class DEPRIVATIONGAME_API AChatExample : public AActor
{
	GENERATED_BODY()
	
public:	
	AChatExample();

protected:
	virtual void BeginPlay() override;

public:
	// Пример функции для отправки сообщения игроку
	UFUNCTION(BlueprintCallable, Category = "Chat Example")
	void SendTaskCompletionMessage();

	// Пример функции для ответа игрока
	UFUNCTION(BlueprintCallable, Category = "Chat Example")
	void SendPlayerReply(const FString& ReplyText);
};