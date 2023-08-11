// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceRogueAnimInstance.h"
#include "SpaceRogueCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"
#include "WeaponType.h"


USpaceRogueAnimInstance::USpaceRogueAnimInstance():
	
	EquippedWeaponType(EWeaponType::EWT_MAX),
	bShouldUseFABRIK(false)

{}
	

void USpaceRogueAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (SpaceRogueCharacter == nullptr)
	{
		SpaceRogueCharacter = Cast<ASpaceRogueCharacter>(TryGetPawnOwner());
	}

	if (SpaceRogueCharacter)
	{
		bShouldUseFABRIK = SpaceRogueCharacter->GetCombatState() == ECombatState::ECS_Unoccupied || SpaceRogueCharacter->GetCombatState() == ECombatState::ECS_FireTimerInProgress;

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

		FRotator AimRotation = SpaceRogueCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(SpaceRogueCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
		
		if (SpaceRogueCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		bAiming = SpaceRogueCharacter->GetAiming();
		//check if character has a valid equipped weapon
		if (SpaceRogueCharacter->GetEquippedWeapon())
		{
			EquippedWeaponType = SpaceRogueCharacter->GetEquippedWeapon()->GetWeaponType();
		}
		
	}
	
}

void USpaceRogueAnimInstance::NativeInitializeAnimation()
{
	SpaceRogueCharacter = Cast<ASpaceRogueCharacter>(TryGetPawnOwner());
}

