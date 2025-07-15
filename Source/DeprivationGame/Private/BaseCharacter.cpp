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
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
