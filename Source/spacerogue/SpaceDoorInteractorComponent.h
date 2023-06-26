// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceDoorInteractorComponent.generated.h"

class ATriggerBox;

UENUM()
enum class EDoorState
{
	DS_Closed = 0		UMETA(DisplayName = "Closed"),
	DS_Opening = 1		UMETA(DisplayName = "Opening"),
	DS_Open = 2		UMETA(DisplayName = "Open"),
	DS_Closing=3		UMETA(DisplayName = "Closing"),
	DS_Locked = 4		UMETA(DisplayName = "Locked"),

};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEROGUE_API USpaceDoorInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpaceDoorInteractorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere)
		ATriggerBox* TriggerBox;

	
	FVector StartLocation;
	FVector MoveDirection;

	float DoorPosition;
	UPROPERTY(EditAnywhere)
	float DoorOpenPosition = 380;
	UPROPERTY(EditAnywhere)
	float DoorClosedPosition = 0;
	UPROPERTY(EditAnywhere)
	float Speed = 1;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor();
	void CloseDoor();

	UPROPERTY(BlueprintReadOnly)
		EDoorState DoorState;
		
};
