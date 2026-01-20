#include "BaseCharacter.h"
#include "Vehicle/DeprivationCar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Interactable/PickableItemActor.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	
	ItemHoldSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ItemHoldSocket"));
	ItemHoldSocket->SetupAttachment(CameraComponent);

	StateComponent = CreateDefaultSubobject<UCharacterStateComponent>(TEXT("StateComponent"));
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));


	CurrentHoveredInteractable = nullptr;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CameraComponent = FindComponentByClass<UCameraComponent>();
	if (InteractableComponent)
	{
		InteractableComponent->OnInteractionStarted.AddDynamic(this, &ABaseCharacter::HandleInteractionStarted);
		InteractableComponent->OnInteractionCompleted.AddDynamic(this, &ABaseCharacter::HandleInteractionCompleted);
		InteractableComponent->OnInteractionCancelled.AddDynamic(this, &ABaseCharacter::HandleInteractionCancelled);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::PickUpItem(APickableItemActor* ItemToPick)
{
	if (blsHoldItem)
	{
		blsHoldItem->ReturnToOriginalPosition();
		blsHoldItem = nullptr;
	}

	blsHoldItem = ItemToPick;

	blsHoldItem->SaveOriginalTransform();

	blsHoldItem->SetIsHeld(true);
	blsHoldItem->SetActorEnableCollision(false);

	blsHoldItem->AttachToComponent(ItemHoldSocket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	blsHoldItem->SetActorRelativeLocation(blsHoldItem->GetHandOffset());
	blsHoldItem->SetActorRelativeRotation(blsHoldItem->GetHandRotation());

	blsHoldItem->SetActorHiddenInGame(false);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Interaction binding
		if (InteractionAction)
		{
			EnhancedInput->BindAction(InteractionAction, ETriggerEvent::Started, this, &ABaseCharacter::Interact);
		}

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				if (CharacterMappingContext)
				{
					Subsystem->AddMappingContext(CharacterMappingContext, 0);
				}
			}
		}
	}
}

void ABaseCharacter::EnterVehicle(APawn* Vehicle)
{
	ADeprivationCar* Car = Cast<ADeprivationCar>(Vehicle);

	CurrentVehicle = Car;
	StateComponent->SetState(ECharacterState::InVehicle);

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->SetMovementMode(MOVE_None);
		MovementComp->StopMovementImmediately();
		MovementComp->SetComponentTickEnabled(false);
	}

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	AttachToComponent(Car->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (AController* MyController = GetController())
	{
		MyController->UnPossess();
		MyController->Possess(Vehicle);
	}

	Car->EnterVehicle(Cast<APawn>(this));
}

void ABaseCharacter::ExitVehicle()
{
	Cast<ADeprivationCar>(CurrentVehicle)->ExitVehicle();
	CurrentVehicle = nullptr;
	StateComponent->SetState(ECharacterState::Normal);
}

void ABaseCharacter::EnterVehicleByTag(FName VehicleTag)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), VehicleTag, FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ADeprivationCar* Car = Cast<ADeprivationCar>(Actor))
		{
			EnterVehicle(Car);
			return;
		}
	}
}


AActor* ABaseCharacter::LineTrace(float LineLength, bool bDrawDebug) const
{
	FVector Start = CameraComponent ? CameraComponent->GetComponentLocation() : GetActorLocation();
	FVector End = Start + (CameraComponent ? CameraComponent->GetForwardVector() : GetActorForwardVector()) *
		LineLength;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (bDrawDebug)
	{
		FColor Color = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), Start, End, Color, false, 2.0f, 0, 2.0f);
		if (bHit) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, Color, false, 2.0f);
	}

	return bHit ? Hit.GetActor() : nullptr;
}


void ABaseCharacter::Interact()
{
	if (InteractableComponent)
	{
		InteractableComponent->Interact();
	}
}

void ABaseCharacter::HandleInteractionStarted(AActor* InteractableActor, ABaseCharacter* Character)
{
	UE_LOG(LogTemp, Log, TEXT("Interaction started with %s"), *InteractableActor->GetName());
}

void ABaseCharacter::HandleInteractionCompleted(AActor* InteractableActor, ABaseCharacter* Character)
{
	UE_LOG(LogTemp, Log, TEXT("Interaction completed with %s"), *InteractableActor->GetName());
}

void ABaseCharacter::HandleInteractionCancelled(AActor* InteractableActor, ABaseCharacter* Character)
{
	UE_LOG(LogTemp, Log, TEXT("Interaction cancelled with %s"), *InteractableActor->GetName());
}
void ABaseCharacter::StartChoppingMinigame()
{
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractionCheckTimerHandle);
	}
}
