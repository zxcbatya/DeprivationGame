
void ABackpackActor::HandleBackpackInteraction(APawn* InteractingPawn)
{
	if (!bIsOpened && BackpackMeshOpened)
	{
		bIsOpened = true;
		BackpackMesh->SetStaticMesh(BackpackMeshOpened);
	}
	
	if (bIsPlacedAsContainer)
	{
		
		if (!bHasToothbrush || !bHasClothes)
		{
			ProcessNormalItemRetrieval(InteractingPawn);
			return;
		}
		
		if (!bStorySequenceActive && BackpackItems.Num() == 0 && CurrentItemIndex >= 2)
		{
			StartNarrativeSequence(InteractingPawn);
			return;
		}
	}

	Super::OnInteract_Implementation(InteractingPawn);
}

void ABackpackActor::ProcessNormalItemRetrieval(APawn* InteractingPawn)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
	{
		if (Character->GetHoldItem() != nullptr)
		{
			return;
		}
	}
	
	AItemActor* Item = GetNextItem();

	if (Item && InteractingPawn)
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingPawn))
		{
			Character->PickUpItem(Item);
			
			if (Item->GetItemType() == EItemType::TOOTHBRUSHANDPASTE)
			{
				bHasToothbrush = true;
			}
			else if (Item->GetItemType() == EItemType::CLOTHES)
			{
				bHasClothes = true;
			}
		}
		OnItemRemovedFromBackpack(Item, InteractingPawn);
	}
}

void ABackpackActor::StartNarrativeSequence(APawn* InteractingPawn)
{
	bStorySequenceActive = true;
	StoryInteractingPawn = InteractingPawn;
	StoryStep = 0;
	
	OnNarrativeSequenceStarted.Broadcast(InteractingPawn);
	
	// Запускаем первый этап
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		ExecuteNarrativeStep(EStoryStep::AlcoholBottle);
	}, 0.1f, false);
}

void ABackpackActor::ExecuteNarrativeStep(EStoryStep Step)
{
	switch (Step)
	{
	case EStoryStep::AlcoholBottle:
		SpawnTemporaryItem(EItemType::BOTTLE);
		// Через 4 секунды предмет исчезает
		GetWorld()->GetTimerManager().SetTimer(StoryItemTimerHandle, [this]()
		{
			ClearTemporaryItem();
			CompleteCurrentStep();
		}, 4.0f, false);
		break;
		
	case EStoryStep::Ziplock:
		SpawnTemporaryItem(EItemType::ZIPLOCK);
		// Через 3 секунды предмет исчезает
		GetWorld()->GetTimerManager().SetTimer(StoryItemTimerHandle, [this]()
		{
			ClearTemporaryItem();
			CompleteCurrentStep();
		}, 3.0f, false);
		break;
		
	case EStoryStep::Complete:
		EndNarrativeSequence();
		break;
	}
}

void ABackpackActor::CompleteCurrentStep()
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(StoryInteractingPawn))
	{
		if (Character->InteractableComponent)
		{
			Character->InteractableComponent->SetHoldItem(nullptr);
			Character->blsHoldItem = nullptr;
		}
	}
	
	StoryStep++;
	
	// Определяем следующий шаг
	EStoryStep NextStep;
	switch (StoryStep)
	{
	case 1: NextStep = EStoryStep::AlcoholBottle; break;
	case 2: NextStep = EStoryStep::Ziplock; break;
	default: NextStep = EStoryStep::Complete; break;
	}
	
	ExecuteNarrativeStep(NextStep);
}

void ABackpackActor::EndNarrativeSequence()
{
	bStorySequenceActive = false;
	StoryStep = 0;
	
	OnNarrativeSequenceEnded.Broadcast(StoryInteractingPawn);
	
	StoryInteractingPawn = nullptr;
}

void ABackpackActor::ClearTemporaryItem()
{
	if (HeldStoryItem)
	{
		HeldStoryItem->Destroy();
		HeldStoryItem = nullptr;
	}
	
	// Очищаем руку персонажа
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(StoryInteractingPawn))
	{
		if (Character->InteractableComponent)
		{
			Character->InteractableComponent->SetHoldItem(nullptr);
		}
		Character->blsHoldItem = nullptr;
	}
}
