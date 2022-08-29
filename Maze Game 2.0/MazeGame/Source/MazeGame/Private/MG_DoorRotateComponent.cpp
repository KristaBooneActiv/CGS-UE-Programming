// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_DoorRotateComponent.h"
#include "Engine/Classes/Components/ShapeComponent.h"
#include "Engine/TriggerBox.h"

// Sets default values for this component's properties
UMG_DoorRotateComponent::UMG_DoorRotateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UMG_DoorRotateComponent::BeginPlay()
{
	Super::BeginPlay();
	PrimaryComponentTick.SetTickFunctionEnable(false);

	if (TriggerBox)
	{
		TriggerBox->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &UMG_DoorRotateComponent::OnOverlapBegin);

		StartRotation = GetOwner()->GetActorRotation();
		FinalRotation = GetOwner()->GetActorRotation() + DesiredRotation;
		CurrentRotationTime = 0.0f;
	}
}

// Called every frame
void UMG_DoorRotateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ExecuteRotation(DeltaTime))
	{
		UE_LOG(LogTemp, Display, TEXT("UMG_DoorRotateComponent:: Finished rotation"));
		PrimaryComponentTick.SetTickFunctionEnable(false);
	}
}

void UMG_DoorRotateComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("UMG_DoorRotateComponent:: Player overlapped with door trigger"));
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

bool UMG_DoorRotateComponent::ExecuteRotation(float DeltaTime)
{
	// If we are not done rotating, rotate.
	if (CurrentRotationTime < TimeToRotate)
	{
		CurrentRotationTime += DeltaTime;

		const float TimeRatio = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.0f, 1.0f);
		const FRotator CurrentRotation = FMath::Lerp(StartRotation, FinalRotation, TimeRatio);
		GetOwner()->SetActorRotation(CurrentRotation);

		return false; // not done
	}
	return true; // done
}
