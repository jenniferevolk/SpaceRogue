// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceDoorInteractorComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerBox.h"
#include "Engine/World.h"

// Sets default values for this component's properties
USpaceDoorInteractorComponent::USpaceDoorInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpaceDoorInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetOwner()->GetActorLocation();
	DoorPosition = StartLocation.Z;
	// ...
	
}


// Called every frame
void USpaceDoorInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{




	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DoorState == EDoorState::DS_Closed)
	{
		if (TriggerBox && GetWorld()->GetFirstLocalPlayerFromController())
		{
			APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
			if (PlayerPawn && TriggerBox->IsOverlappingActor(PlayerPawn))
			{
				DoorState = EDoorState::DS_Opening;
				UE_LOG(LogTemp, Warning, TEXT("Door Triggered:Opening"));
			}
		}
	}
	
	if (DoorState == EDoorState::DS_Open)
	{

		if (TriggerBox && GetWorld()->GetFirstLocalPlayerFromController())
		{
			APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
			if (PlayerPawn && !TriggerBox->IsOverlappingActor(PlayerPawn))
			{
				DoorState = EDoorState::DS_Closing;
				UE_LOG(LogTemp, Warning, TEXT("Door Triggered: Closing"));
			}
		}

	}


	if (DoorState ==EDoorState::DS_Opening)
	{
		OpenDoor();
	}
	if (DoorState == EDoorState::DS_Closing)
	{
		CloseDoor();
	}
	
	
}

void USpaceDoorInteractorComponent::OpenDoor()
{

	StartLocation = GetOwner()->GetActorLocation();
	MoveDirection = FVector(0, 0, 1.0f) * Speed;
	StartLocation = StartLocation + MoveDirection;
	DoorPosition = StartLocation.Z;

	if (DoorPosition <DoorOpenPosition)
	{
		GetOwner()->SetActorLocation(StartLocation);
	}
	else
	{
		DoorState = EDoorState::DS_Open;
		StartLocation = FVector(StartLocation.X, StartLocation.Y, DoorOpenPosition);
		GetOwner()->SetActorLocation(StartLocation);
	}
}
void USpaceDoorInteractorComponent::CloseDoor()
{

	StartLocation = GetOwner()->GetActorLocation();
	MoveDirection = FVector(0, 0, -1.0f) * Speed;
	StartLocation = StartLocation + MoveDirection;
	DoorPosition = StartLocation.Z;

	if (DoorPosition > DoorClosedPosition)
	{
		GetOwner()->SetActorLocation(StartLocation);
	}
	else
	{
		DoorState = EDoorState::DS_Closed;
		StartLocation = FVector(StartLocation.X, StartLocation.Y, DoorClosedPosition);
		GetOwner()->SetActorLocation(StartLocation);
	}
}