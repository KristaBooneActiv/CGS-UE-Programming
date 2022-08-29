// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_DoorTranslateComponent.h"
#include "Engine/Classes/Components/ShapeComponent.h"
#include "Engine/TriggerBox.h"

// Sets default values for this component's properties
UMG_DoorTranslateComponent::UMG_DoorTranslateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UMG_DoorTranslateComponent::BeginPlay()
{
	Super::BeginPlay();
	PrimaryComponentTick.SetTickFunctionEnable(false);

	if (TriggerBox)
	{
		TriggerBox->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &UMG_DoorTranslateComponent::OnOverlapBegin);

		// Set up start position by getting actor's X, Y, or Z depending on the dimension
		// the translation will be applied to.
		switch (TranslationDimension)
		{
		case ETranslationDimensionType::X:
		{
			StartPosition = GetOwner()->GetActorLocation().X;
			break;
		}
		case ETranslationDimensionType::Y:
		{
			StartPosition = GetOwner()->GetActorLocation().Y;
			break;
		}
		case ETranslationDimensionType::Z:
		{
			StartPosition = GetOwner()->GetActorLocation().Z;
			break;
		}
		default:
			// TODO: ERROR
			break;
		}

		FinalPosition = StartPosition + TranslationAmount;
		CurrentTranslationTime = 0.0f;
	}
}

// Called every frame
void UMG_DoorTranslateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (ExecuteTranslation(DeltaTime))
	{
		UE_LOG(LogTemp, Display, TEXT("UMG_DoorTranslateComponent:: Finished translation"));
		PrimaryComponentTick.SetTickFunctionEnable(false);
	}
}

void UMG_DoorTranslateComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("UMG_DoorTranslateComponent:: Player overlapped with door trigger"));
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

bool UMG_DoorTranslateComponent::ExecuteTranslation(float DeltaTime)
{
	// If we're not done translating, execute.
	if (CurrentTranslationTime < TimeToTranslate)
	{
		CurrentTranslationTime += DeltaTime;

		const float TimeRatio = FMath::Clamp(CurrentTranslationTime / TimeToTranslate, 0.0f, 1.0f);
		const float CurrentPosition = FMath::Lerp(StartPosition, FinalPosition, TimeRatio);

		switch (TranslationDimension)
		{
		case ETranslationDimensionType::X:
		{
			GetOwner()->SetActorLocation(FVector(
				CurrentPosition,
				GetOwner()->GetActorLocation().Y,
				GetOwner()->GetActorLocation().Z)
			);
			break;
		}
		case ETranslationDimensionType::Y:
		{
			GetOwner()->SetActorLocation(FVector(
				GetOwner()->GetActorLocation().X,
				CurrentPosition,
				GetOwner()->GetActorLocation().Z)
			);
			break;
		}
		case ETranslationDimensionType::Z:
		{
			GetOwner()->SetActorLocation(FVector(
				GetOwner()->GetActorLocation().X,
				GetOwner()->GetActorLocation().Y,
				CurrentPosition)
			);
			break;
		}
		default:
			// TODO: ERROR
			break;
		}

		return false; // Not done.
	}

	return true; // done
}
