// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MG_DoorTranslateComponent.generated.h"

class ATriggerBox;

UENUM(BlueprintType)
enum class ETranslationDimensionType : uint8
{
	X UMETA(DisplayName = "X"),
	Y UMETA(DisplayName = "Y"),
	Z UMETA(DisplayName = "Z")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAZEGAME_API UMG_DoorTranslateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMG_DoorTranslateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
			            AActor* OtherActor,
			            UPrimitiveComponent* OtherComp,
			            int32 OtherBodyIndex,
			            bool bFromSweep,
			            const FHitResult& SweepResult);

	// Returns true when finished
	bool ExecuteTranslation(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float TranslationAmount = 2.0f;
	float StartPosition = 0.0f;
	float FinalPosition = 0.0f;

	UPROPERTY(EditAnywhere)
	float TimeToTranslate = 1.0f;
	float CurrentTranslationTime = 0.0f;

	UPROPERTY(EditAnywhere)
	ETranslationDimensionType TranslationDimension = ETranslationDimensionType::X;

	UPROPERTY(EditAnywhere)
	ATriggerBox* TriggerBox = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
