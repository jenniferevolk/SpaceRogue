// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceRogueAnimInstance.h"
#include "SpaceRogueCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void USpaceRogueAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (SpaceRogueCharacter == nullptr)
	{
		SpaceRogueCharacter = Cast<ASpaceRogueCharacter>(TryGetPawnOwner());
	}

	if (SpaceRogueCharacter)
	{
		FVector Velocity{ SpaceRogueCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = SpaceRogueCharacter->GetCharacterMovement()->IsFalling();

		if (SpaceRogueCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

	}
}

void USpaceRogueAnimInstance::NativeInitializeAnimation()
{
	SpaceRogueCharacter = Cast<ASpaceRogueCharacter>(TryGetPawnOwner());
}

