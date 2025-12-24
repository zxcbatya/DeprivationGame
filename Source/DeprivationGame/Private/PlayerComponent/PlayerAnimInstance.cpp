// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponent/PlayerAnimInstance.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CharacterPawn = TryGetPawnOwner();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CharacterPawn != nullptr)
	{
		Speed = CharacterPawn->GetVelocity().Size();
	}
}
