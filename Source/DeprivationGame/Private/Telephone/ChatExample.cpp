// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/ChatExample.h"
#include "Telephone/ChatManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

AChatExample::AChatExample()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AChatExample::BeginPlay()
{
	Super::BeginPlay();
}

void AChatExample::SendTaskCompletionMessage()
{
	// Получаем подсистему чатов
	UChatManagerSubsystem* ChatSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UChatManagerSubsystem>();
	if (!ChatSubsystem)
		return;

	// Добавляем контакт, если его еще нет
	ChatSubsystem->AddContact(1001, FText::FromString("Система"), ""); // Изменили ID

	// Отправляем сообщение о выполнении задания
	ChatSubsystem->SendMessage(1001, "system", "Вы успешно выполнили задание 'Умыться'", ""); // Изменили ID
}

void AChatExample::SendPlayerReply(const FString& ReplyText)
{
	// Получаем подсистему чатов
	UChatManagerSubsystem* ChatSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UChatManagerSubsystem>();
	if (!ChatSubsystem)
		return;

	// Отправляем ответ игрока
	ChatSubsystem->SendMessage(1001, "player", ReplyText, ""); // Изменили ID
}