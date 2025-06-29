#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::SetFatigueState(EFatigueState NewState)
{
	EFatigueState CurrentFatigueState = {};
	if (CurrentFatigueState == NewState) return; 
    
	CurrentFatigueState = NewState;
    
	OnFatigueStateChanged(CurrentFatigueState);
}

void ABaseCharacter::SetDrunkState(EDrunkState NewState)
{
	EDrunkState DrunkState = {};
	if (NewState == DrunkState)
	{
		DrunkState = NewState;
		OnDrunkStateChanged(DrunkState);
	}
}

inline void ABaseCharacter::OnDrunkStateChanged( EDrunkState State)
{
	switch (State) {
	case EDrunkState::Sober:
		// Активировать нормальную модель лица
			break;
	case EDrunkState::Tipsy:
		// Активировать модель лица с мешками под глазами
			break;
	case EDrunkState::Drunk:
		// Активировать модель лица с более выраженными мешками
			break;
	case EDrunkState::Wasted:
		// Активировать модель лица с трясущимися глазами
			break;
	}
}

inline void ABaseCharacter::OnFatigueStateChanged(EFatigueState State)
{
	switch (State)
	{
	case EFatigueState::Fresh:
		// Активировать нормальную модель лица
			break;
	case EFatigueState::Tired:
		// Активировать модель лица с мешками под глазами
			break;
	case EFatigueState::Exhausted:
		// Активировать модель лица с более выраженными мешками
			break;
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
