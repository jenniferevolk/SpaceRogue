// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SpaceRogueAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACEROGUE_API USpaceRogueAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Movement,meta=(AllowPrivateAccess="true"))
	class ASpaceRogueCharacter* SpaceRogueCharacter;

	UPROPERTY(VisibleAnyWhere,BlueprintReadOnly,Category=Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

};
