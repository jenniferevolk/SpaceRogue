// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceRoguePlayerController.h"
#include "Blueprint/UserWidget.h"

ASpaceRoguePlayerController::ASpaceRoguePlayerController()
{

}

void ASpaceRoguePlayerController::BeginPlay()
{
	Super::BeginPlay();

	//check our hud overlayclass tsubclassof variable
	if (HUDOverlayClass)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
		if (HUDOverlay)
		{
			HUDOverlay->AddToViewport();
			HUDOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
