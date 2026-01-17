#include "Interactable/ItemActor.h"

AItemActor::AItemActor()
{
	// Конструктор по умолчанию
	ItemType = EItemType::NONE;
	TargetPlacementZone = nullptr;
}