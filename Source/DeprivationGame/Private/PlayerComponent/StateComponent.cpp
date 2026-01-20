#include "PlayerComponent/StateComponent.h"

UCharacterStateComponent::UCharacterStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentState = ECharacterState::Normal;
}

void UCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterStateComponent::SetState(ECharacterState NewState)
{
		CurrentState = NewState;
		OnCharacterStateChanged.Broadcast(NewState);
}

ECharacterState UCharacterStateComponent::GetCurrentState() const
{
	return CurrentState;
}
