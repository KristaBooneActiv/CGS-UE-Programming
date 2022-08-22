// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MG_MT_DoorOpenSlideComponent.generated.h"

class ATriggerBox;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAZEGAME_API UMG_MT_DoorOpenSlideComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMG_MT_DoorOpenSlideComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool HandleTriggerCheck();

	UPROPERTY(EditAnywhere)
	float SlideDistance = 2.0f;
	float StartPosition = 0.0f;
	float FinalPosition = 0.0f;

	UPROPERTY(EditAnywhere)
	float TimeToSlide = 1.0f;
	float CurrentSlideTime = 0.0f;

	UPROPERTY(EditAnywhere)
	ATriggerBox* TriggerBox1;

	UPROPERTY(EditAnywhere)
	ATriggerBox* TriggerBox2;

	UPROPERTY(EditAnywhere)
	ATriggerBox* TriggerBox3;

	bool AllTriggeredInOrder = false;
	bool HasTriggered1 = false;
	bool HasTriggered2 = false;
	bool HasTriggered3 = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
