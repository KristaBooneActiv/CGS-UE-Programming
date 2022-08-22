// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_DoorOpenUpComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerBox.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UMG_DoorOpenUpComponent::UMG_DoorOpenUpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMG_DoorOpenUpComponent::BeginPlay()
{
	Super::BeginPlay();

	StartHeight = GetOwner()->GetActorLocation().Z;
	FinalHeight = StartHeight + RaiseHeight;
	CurrentLiftTime = 0.0f;
}


// Called every frame
void UMG_DoorOpenUpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentLiftTime < TimeToLift)
	{
		// If the lift has not been triggered, try to see if it is now triggered
		if (!HasTriggered)
		{
			if (TriggerBox && GetWorld() && GetWorld()->GetFirstLocalPlayerFromController())
			{
				APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
				if (PlayerPawn && TriggerBox->IsOverlappingActor(PlayerPawn))
				{
					HasTriggered = true;
				}
			}
		}
		// If the lift has been triggered, execute.
		else
		{
			CurrentLiftTime += DeltaTime;

			const float TimeRatio = FMath::Clamp(CurrentLiftTime / TimeToLift, 0.0f, 1.0f);
			const float CurrentHeight = FMath::Lerp(StartHeight, FinalHeight, TimeRatio);
			GetOwner()->SetActorLocation(FVector(
				GetOwner()->GetActorLocation().X,
				GetOwner()->GetActorLocation().Y,
				CurrentHeight)
			);
		}
	}
}

