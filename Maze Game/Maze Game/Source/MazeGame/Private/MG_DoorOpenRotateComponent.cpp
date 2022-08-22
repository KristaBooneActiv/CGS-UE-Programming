// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_DoorOpenRotateComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerBox.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UMG_DoorOpenRotateComponent::UMG_DoorOpenRotateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMG_DoorOpenRotateComponent::BeginPlay()
{
	Super::BeginPlay();
	StartRotation = GetOwner()->GetActorRotation();
	FinalRotation = GetOwner()->GetActorRotation() + DesiredRotation;
	CurrentRotationTime = 0.0f;
}


// Called every frame
void UMG_DoorOpenRotateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentRotationTime < TimeToRotate)
	{
		// If the rotation has not been triggered, try to see if it is now triggered
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
		// If the rotation has been triggered, execute.
		else
		{
			CurrentRotationTime += DeltaTime;

			const float TimeRatio = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.0f, 1.0f);
			const FRotator CurrentRotation = FMath::Lerp(StartRotation, FinalRotation, TimeRatio);
			GetOwner()->SetActorRotation(CurrentRotation);
		}
	}
}

