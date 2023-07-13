// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceRoguePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEROGUE_API ASpaceRoguePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASpaceRoguePlayerController();


protected:

	virtual void BeginPlay() override;

private:
	/** reference to overall HUD overlay blueprint class */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Widgets,meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UUserWidget>HUDOverlayClass;

	/** variable to hold the hud overlay widget  */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDOverlay;
};
