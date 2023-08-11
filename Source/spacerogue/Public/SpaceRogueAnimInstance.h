// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponType.h"
#include "SpaceRogueAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACEROGUE_API USpaceRogueAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	USpaceRogueAnimInstance();
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement,meta=(AllowPrivateAccess="true"))
	class ASpaceRogueCharacter* SpaceRogueCharacter;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	/** offset yaw used for strafing */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;
	

	/** offset yaw used for strafing */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float LastMovementOffsetYaw;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EWeaponType EquippedWeaponType;

	//dont use fabrik when equipping or reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bShouldUseFABRIK;


};
