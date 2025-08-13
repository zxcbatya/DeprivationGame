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
	EFatigueState CurrentFatigueState = {}; // Это неправильно - нужно использовать член класса
	if (TiredState == NewState) return; 
    
	TiredState = NewState;
    
	OnFatigueStateChanged(TiredState);
}

void ABaseCharacter::SetDrunkState(EDrunkState NewState)
{
	if (DrunkState == NewState) return;
    
	DrunkState = NewState;
    
	OnDrunkStateChanged(DrunkState);
}

void ABaseCharacter::OnFatigueStateChanged(EFatigueState State)
{
	// Здесь можно добавить логику для изменения состояния усталости
	// Например, изменение материалов, звуков и т.д.
}

void ABaseCharacter::OnDrunkStateChanged(EDrunkState State)
{
	// Здесь можно добавить логику для изменения состояния опьянения
	// Например, изменение материалов, звуков и т.д.
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
