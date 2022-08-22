// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_MT_DoorOpenSlideComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerBox.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UMG_MT_DoorOpenSlideComponent::UMG_MT_DoorOpenSlideComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMG_MT_DoorOpenSlideComponent::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetOwner()->GetActorLocation().X;
	FinalPosition = StartPosition + SlideDistance;
	CurrentSlideTime = 0.0f;	
}


// Called every frame
void UMG_MT_DoorOpenSlideComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentSlideTime < TimeToSlide)
	{
		// If the slide has not been triggered, try to see if it is now triggered
		if (HandleTriggerCheck())
		{
			// If the slide has been triggered, execute.
			CurrentSlideTime += DeltaTime;

			const float TimeRatio = FMath::Clamp(CurrentSlideTime / TimeToSlide, 0.0f, 1.0f);
			const float CurrentX = FMath::Lerp(StartPosition, FinalPosition, TimeRatio);
			GetOwner()->SetActorLocation(FVector(
				CurrentX,
				GetOwner()->GetActorLocation().Y,
				GetOwner()->GetActorLocation().Z)
			);
		}
	}
}

bool UMG_MT_DoorOpenSlideComponent::HandleTriggerCheck()
{
	// Check if this has already been triggered
	if (AllTriggeredInOrder)
	{
		return true; 
	}

	// Verify world state and controller are good
	if (!GetWorld() || !GetWorld()->GetFirstLocalPlayerFromController())
	{
		return false;
	}

	// Verify PlayerPawn is available
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
	{
		return false;
	}

	// See if player is in any triggerbox
	bool inTrigger1 = TriggerBox1->IsOverlappingActor(PlayerPawn);
	bool inTrigger2 = TriggerBox2->IsOverlappingActor(PlayerPawn);
	bool inTrigger3 = TriggerBox3->IsOverlappingActor(PlayerPawn);

	// Verify order of trigger events
	if (inTrigger1)
	{
		HasTriggered1 = !HasTriggered2 && !HasTriggered3;
		return false;
	}
	else if (inTrigger2)
	{
		HasTriggered2 = HasTriggered1 && !HasTriggered3;
		return false;
	}
	else if (inTrigger3)
	{
		HasTriggered3 = HasTriggered1 && HasTriggered2;
		AllTriggeredInOrder = HasTriggered3;
		return AllTriggeredInOrder;
	}
	else
	{
		return false;
	}
}