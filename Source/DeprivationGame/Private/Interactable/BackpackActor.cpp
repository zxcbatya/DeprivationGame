#include "Interactable/BackpackActor.h"
#include "GameFramework/Pawn.h"
#include "BaseCharacter.h"
#include "Interactable/PlacementZone.h"

ABackpackActor::ABackpackActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Инициализируем валидные зоны размещения
	static ConstructorHelpers::FClassFinder<APlacementZone> TableFinder(TEXT("/Game/Blueprints/PlacementZones/BP_TableZone"));  
	if (TableFinder.Class) ValidPlacementZones.Add(TableFinder.Class);
	
	static ConstructorHelpers::FClassFinder<APlacementZone> ChairFinder(TEXT("/Game/Blueprints/PlacementZones/BP_ChairZone"));
	if (ChairFinder.Class) ValidPlacementZones.Add(ChairFinder.Class);
	
	static ConstructorHelpers::FClassFinder<APlacementZone> SofaFinder(TEXT("/Game/Blueprints/PlacementZones/BP_SofaZone"));
	if (SofaFinder.Class) ValidPlacementZones.Add(SofaFinder.Class);
}

bool ABackpackActor::CanInteract_Implementation(APawn* InteractingPawn) const
{
	UE_LOG(LogTemp, Warning, TEXT("=== Backpack CanInteract ==="));
	UE_LOG(LogTemp, Warning, TEXT("bIsPlacedAsContainer: %s"), bIsPlacedAsContainer ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("HasMoreItems: %s"), HasMoreItems() ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("bItemsLocked: %s"), bItemsLocked ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("BackpackItems.Num(): %d"), BackpackItems.Num());
	UE_LOG(LogTemp, Warning, TEXT("CurrentItemIndex: %d"), CurrentItemIndex);
	
	// Если рюкзак уже размещён как контейнер
	if (bIsPlacedAsContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Container mode - checking items availability"));
		// Разрешаем взаимодействие только если есть предметы для извлечения
		// и они не заблокированы
		bool Result = HasMoreItems() && !bItemsLocked;
		UE_LOG(LogTemp, Warning, TEXT("CanInteract result: %s"), Result ? TEXT("true") : TEXT("false"));
		return Result;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Normal pickup mode"));
	// Для обычного подбора используем родительскую логику
	return Super::CanInteract_Implementation(InteractingPawn);
}

void ABackpackActor::OnInteract_Implementation(APawn* InteractingPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("BACKPACK OnInteract called - processing interaction"));
	
	// Основная логика обработки
	HandleBackpackInteraction(InteractingPawn);
}

void ABackpackActor::HandleBackpackInteraction(APawn* InteractingPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("=== Backpack HandleBackpackInteraction ==="));
	UE_LOG(LogTemp, Warning, TEXT("bIsPlacedAsContainer: %s"), bIsPlacedAsContainer ? TEXT("true") : TEXT("false"));
	
	// Если рюкзак размещён как контейнер
	if (bIsPlacedAsContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Container mode - extracting item"));
		
		// Извлекаем следующий предмет
		AItemActor* Item = GetNextItem();
		UE_LOG(LogTemp, Warning, TEXT("GetNextItem returned: %s"), Item ? *Item->GetName() : TEXT("nullptr"));
		
		if (Item && InteractingPawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("Giving item to player"));
			
			// Отдаём предмет игроку в руки
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
			{
				Character->PickUpItem(Item);
			}
			
			// Вызываем событие извлечения
			OnItemRemovedFromBackpack(Item, InteractingPawn);
		}
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Normal pickup mode"));
	// Для обычного подбора используем родительскую логику
	Super::OnInteract_Implementation(InteractingPawn);
}

AItemActor* ABackpackActor::GetNextItem()
{
	if (bItemsLocked || CurrentItemIndex >= BackpackItems.Num())
	{
		return nullptr;
	}
	
	AItemActor* Item = BackpackItems[CurrentItemIndex];
	CurrentItemIndex++;
	
	if (Item)
	{
		// Показываем предмет (если был скрыт)
		Item->SetActorHiddenInGame(false);
		Item->SetIsHeld(true);
	}
	
	return Item;
}

void ABackpackActor::InitializeBackpackContents()
{
	BackpackItems.Empty();
	CurrentItemIndex = 0;
	
	// Создаем экземпляры предметов из классов
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

void ABackpackActor::AddItemToBackpack(AItemActor* Item)
{
	if (Item)
	{
		BackpackItems.Add(Item);
		Item->SetActorHiddenInGame(true);
		Item->SetIsHeld(false);
	}
}

AItemActor* ABackpackActor::RemoveItemFromBackpack(int Index)
{
	if (Index >= 0 && Index < BackpackItems.Num())
	{
		AItemActor* Item = BackpackItems[Index];
		BackpackItems.RemoveAt(Index);
		return Item;
	}
	return nullptr;
}

bool ABackpackActor::IsValidPlacementSurface(APlacementZone* Zone) const
{
	if (!Zone) return false;
	
	// Проверяем через ValidPlacementZones массив
	for (TSubclassOf<APlacementZone> ValidZoneClass : ValidPlacementZones)
	{
		if (Zone->GetClass()->IsChildOf(ValidZoneClass))
		{
			return true;
		}
	}
	return false;
}

