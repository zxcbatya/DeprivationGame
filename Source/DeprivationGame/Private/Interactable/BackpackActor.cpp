#include "Interactable/BackpackActor.h"
#include "GameFramework/Pawn.h"
#include "BaseCharacter.h"
#include "Interactable/PlacementZone.h"
#include "Kismet/GameplayStatics.h"

ABackpackActor::ABackpackActor(): BackpackMesh(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
	BackpackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackpackMesh"));
	BackpackMesh->SetupAttachment(RootComponent);
}

bool ABackpackActor::CanInteract_Implementation(APawn* InteractingPawn) const
{
	if (bIsPlacedAsContainer)
	{
		bool Result = HasMoreItems() && !bItemsLocked;
		return Result;
	}


	return Super::CanInteract_Implementation(InteractingPawn);
}

void ABackpackActor::OnInteract_Implementation(APawn* InteractingPawn)
{
	HandleBackpackInteraction(InteractingPawn);
}

void ABackpackActor::HandleBackpackInteraction(APawn* InteractingPawn)
{
	if (!bIsOpened && BackpackMeshOpened)
	{
		bIsOpened = true;
		BackpackMesh->SetStaticMesh(BackpackMeshOpened);
		FVector Location = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(this, BackpackOpenSound, Location);
	}

	if (bIsPlacedAsContainer)
	{
		// Проверяем, есть ли у персонажа предмет в руке
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
		{
			if (Character->GetHoldItem() != nullptr)
			{
				// Если предмет в руке - не извлекаем следующий
				return;
			}
		}

		AItemActor* Item = GetNextItem();

		if (Item && InteractingPawn)
		{
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
			{
				Character->PickUpItem(Item);
			}
			OnItemRemovedFromBackpack(Item, InteractingPawn);
		}
		return;
	}

	Super::OnInteract_Implementation(InteractingPawn);
}

void ABackpackActor::RemoveItem(APawn* InteractingPawn)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
	{
		Character->SetHoldItem(nullptr);
	}
}

AItemActor* ABackpackActor::GetNextItem()
{
	if (CurrentItemIndex >= BackpackItems.Num())
	{
		return nullptr;
	}

	AItemActor* Item = BackpackItems[CurrentItemIndex];

	if (Item)
	{
		Item->SetActorHiddenInGame(false);
		Item->SetIsHeld(true);
		FVector Location = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(this, BackpackTakeItemSound, Location);
		switch (CurrentItemIndex)
		{
			case 2: TakingVodka();
			UE_LOG(LogTemp, Display, TEXT("Taking Vodka"));
			case 3: TakingDrugs();
			default: break;;
		}
		CurrentItemIndex++; 
	}

	return Item;
}

void ABackpackActor::InitializeBackpackContents()
{
	BackpackItems.Empty();
	CurrentItemIndex = 0;

	for (TSubclassOf<AItemActor> ItemClass : BackpackContentsClasses)
	{
		if (ItemClass && GetWorld())
		{
			FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 100);
			AItemActor* NewItem = GetWorld()->SpawnActor<AItemActor>(ItemClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewItem)
			{
				NewItem->SetActorHiddenInGame(true); // Скрываем до извлечения
				NewItem->SetIsHeld(false);
				BackpackItems.Add(NewItem);
			}
		}
	}
}
