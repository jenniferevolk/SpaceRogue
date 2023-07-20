// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceRogueCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMEshSocket.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "BulletHitInterface.h"
#include "Item.h"
#include "Components/WidgetComponent.h"
#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Ammo.h"
void ASpaceRogueCharacter::FinishReloading()
{
	
	CombatState = ECombatState::ECS_Unoccupied;
	if (EquippedWeapon == nullptr) return;
	const auto AmmoType{ EquippedWeapon->GetAmmoType() };
	if (AmmoMap.Contains(AmmoType))
	{
		//amount of ammo the character is carrying of the equipped weapon type
		int32 CarriedAmmo = AmmoMap[AmmoType];

		//space left in the magazine of equipped weapon
		const int32 MagEmptySpace = EquippedWeapon->GetMagazineCapacity() -
									EquippedWeapon->GetAmmo();

		if (MagEmptySpace > CarriedAmmo)
		{
			//reload the magazine with all the amo we are carrying
			EquippedWeapon->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0;
			AmmoMap.Add(AmmoType, CarriedAmmo);


		}
		else
		{
			//fill the magazine
			EquippedWeapon->ReloadAmmo(MagEmptySpace);
			CarriedAmmo -= MagEmptySpace;
			AmmoMap.Add(AmmoType, CarriedAmmo);

		}
	}
}
void ASpaceRogueCharacter::ResetPickupSoundTimer()
{
	bShouldPlayPickupSound = true;
}
void ASpaceRogueCharacter::ResetEquipSoundTimer()
{
	bShouldPlayEquipSound = true;
}
// Sets default values
ASpaceRogueCharacter::ASpaceRogueCharacter():
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),

	//Camera field of view values
	CameraDefaultFOV(0.f),
	CameraZoomedFOV(35.f),
	CameraCurrentFOV(0.f),
	ZoomInterpSpeed(20.f),
	//turn rates for aiming and not aiming
	HipTurnRate(90.0f),
	HipLookUpRate(90.0f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.f),
	bAiming(false),
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(0.2f),
	MouseAimingLookUpRate(0.2f),
	//crosshair spread factors
	CrosshairSpreadMultiplier(0.f),
	CrosshairVelocityFactor(0.f),
	CrosshairInAirFactor(0.f),
	CrosshairAimFactor(0.f),
	CrosshairShootingFactor(0.f),
	//bullet fire timer variables
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
	bShouldTraceForItems(false),
	//Automatic fire variables
	AutomaticFireRate(0.1f),
	bShouldFire(true),
	bFireButtonPressed(false),
	// camera interp location variables
	CameraInterpDistance(250.f),
	CameraInterpElevation(65.f),
	//starting ammo amounts
	Starting9mmAmmo(85),
	StartingARAmmo(120),
	//combat variables
	CombatState(ECombatState::ECS_Unoccupied),
	bShouldPlayPickupSound(true),
	bShouldPlayEquipSound(true),
	PickupSoundResetTime(0.2f),
	EquipSoundResetTime(0.2f)
	
{
		// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//create a camera boom(pulls in towards the character if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 180.f;       //the camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true;  //rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	//create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);   //attach camera to the end of boom
	FollowCamera->bUsePawnControlRotation = false;  //camera does not rotate relative to arm

	//dont rotate when the controller rotates. let the controller only affect the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false; //character moves in the direction of input..
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); //..at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComp"));
	
	//weapon interpolation component
	WeaponInterpComp = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Interpolation Component"));
	WeaponInterpComp->SetupAttachment(GetFollowCamera());

	//creat interpolation components
	InterpComp1 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 1"));
	InterpComp1->SetupAttachment(GetFollowCamera());

	InterpComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 2"));
	InterpComp2->SetupAttachment(GetFollowCamera());

	InterpComp3 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 3"));
	InterpComp3->SetupAttachment(GetFollowCamera());

	InterpComp4 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 4"));
	InterpComp4->SetupAttachment(GetFollowCamera());

	InterpComp5 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 5"));
	InterpComp5->SetupAttachment(GetFollowCamera());

	InterpComp6 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 6"));
	InterpComp6->SetupAttachment(GetFollowCamera());

}

// Called when the game starts or when spawned
void ASpaceRogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
	EquipWeapon(SpawnDefaultWeapon());
	EquippedWeapon->DisableCustomDepth();
	EquippedWeapon->DisableGlowMaterial();
	InitializeAmmoMap();
	InitializeInterpLocations();
}


bool ASpaceRogueCharacter::GetBeamEndLocation(
	const FVector& MuzzleSocketLocation, 
	FHitResult& OutHitResult)
{
	FVector OutBeamLocation;
	//check for crosshair tracehit
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult,OutBeamLocation);
	if (bCrosshairHit)
	{
		//tenative beam location - still need to trace from gun
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else  //no crosshair hit
	{
		//outbeam location is the end location for the line trace

	}

	//perform trace from gun barrel
	
	const FVector WeaponTraceStart{ MuzzleSocketLocation };
	const FVector StartToEnd{ OutBeamLocation - MuzzleSocketLocation };
	const FVector WeaponTraceEnd{ MuzzleSocketLocation+StartToEnd*1.25f };
	GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		WeaponTraceStart,
		WeaponTraceEnd,
		ECollisionChannel::ECC_Visibility);
	if (!OutHitResult.bBlockingHit) //object between barrel and BeamEndPoint?
	{
		OutHitResult.Location=OutBeamLocation;
		return false;
	}
	return true;
	
}

AWeapon* ASpaceRogueCharacter::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		return GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
		
	}
	return nullptr;
}

void ASpaceRogueCharacter::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip)
	{
		
		//get handsocket
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (HandSocket)
		{
			//attach to hand socket
			HandSocket->AttachActor(WeaponToEquip, GetMesh());
		}
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetItemState(EItemState::EIS_Equipped);

	}
}

void ASpaceRogueCharacter::DropWeapon()
{
	if (EquippedWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);

		EquippedWeapon->SetItemState(EItemState::EIS_Falling);
		EquippedWeapon->ThrowWeapon();
	}
}

void ASpaceRogueCharacter::SelectButtonPressed()
{
	
	if (TraceHitItem)
	{
		TraceHitItem->StartItemCurve(this);
	}
	
}

void ASpaceRogueCharacter::SelectButtonReleased()
{
}

void ASpaceRogueCharacter::SwapWeapon(AWeapon* WeaponToSwap)
{
	DropWeapon();
	EquipWeapon(WeaponToSwap);
	TraceHitItem = nullptr;
	TraceHitItemLastFrame = nullptr;
}

void ASpaceRogueCharacter::PickUpAmmo(AAmmo* Ammo)
{
	if (AmmoMap.Find(Ammo->GetAmmoType()))
	{
		//get amount of ammo in our ammomap for ammos type
		int32 AmmoCount{ AmmoMap[Ammo->GetAmmoType()] };
		AmmoCount += Ammo->GetItemCount();
		//set amount of ammo in ammomap for this type
		AmmoMap[Ammo->GetAmmoType()] = AmmoCount;
	}
	if (EquippedWeapon->GetAmmoType() == Ammo->GetAmmoType())
	{
		//check if gun is empty
		if (EquippedWeapon->GetAmmo() == 0)
		{
			ReloadWeapon();
		}
	}
	Ammo->Destroy();
}

void ASpaceRogueCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	FireWeapon();
}

void ASpaceRogueCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void ASpaceRogueCharacter::StartFireTimer()
{
	CombatState = ECombatState::ECS_FireTimerInProgress;
	GetWorldTimerManager().SetTimer(
		AutoFireTimer,
		this,
		&ASpaceRogueCharacter::AutoFireReset,
		AutomaticFireRate);
	
}

void ASpaceRogueCharacter::AutoFireReset()
{
	CombatState = ECombatState::ECS_Unoccupied;
	if (WeaponHasAmmo())
	{
		if (bFireButtonPressed)
		{
			FireWeapon();
		}
	}
	else
	{
		ReloadWeapon();
	}
}

void ASpaceRogueCharacter::InitializeAmmoMap()
{
	AmmoMap.Add(EAmmoType::EAT_9mm, Starting9mmAmmo);
	AmmoMap.Add(EAmmoType::EAT_AR, StartingARAmmo);
}

bool ASpaceRogueCharacter::WeaponHasAmmo()
{
	if (EquippedWeapon == nullptr) return false;
	return EquippedWeapon->GetAmmo() > 0;
}

void ASpaceRogueCharacter::PlayFireSound()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
}

void ASpaceRogueCharacter::SendBullet()
{
	const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("wehavebarrle socket"));
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetItemMesh());
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		FHitResult BeamHitResult;
		bool bBeamEnd = GetBeamEndLocation(
			SocketTransform.GetLocation(), BeamHitResult);
		if (bBeamEnd)
		{

			if (BeamHitResult.Actor.IsValid())
			{
				IBulletHitInterface* BulletHitInterface = Cast<IBulletHitInterface>(BeamHitResult.Actor.Get());
				if (BulletHitInterface)
				{
					BulletHitInterface->BulletHit_Implementation(BeamHitResult);
				}
			}
			else
			{
				//spawn default particles
				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(),
						ImpactParticles,
						BeamHitResult.Location);
				}

			}


			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				BeamParticles,
				SocketTransform);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamHitResult.Location);
			}
		}


	}
}

void ASpaceRogueCharacter::PlayGunfireMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
	StartCrosshairBulletFire();
}

void ASpaceRogueCharacter::ReloadButtonPressed()
{
	ReloadWeapon();
}

void ASpaceRogueCharacter::ReloadWeapon()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (EquippedWeapon == nullptr) return;
	
	
	if (CarryingAmmo()) 
	{
		
		CombatState = ECombatState::ECS_Reloading;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(EquippedWeapon->GetReloadMontageSection());
		}
	}
}

bool ASpaceRogueCharacter::CarryingAmmo()
{
	if (EquippedWeapon == nullptr) return false;
	auto AmmoType = EquippedWeapon->GetAmmoType();
	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}
	return false;
}

void ASpaceRogueCharacter::GrabClip()
{
	if (EquippedWeapon == nullptr) return;
	if (HandSceneComponent == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("did we make it?"));
	// index for the clip bone on the equipped weapon
	int32 ClipBoneIndex{ EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName()) };
	//store the transform of the clip
	ClipTransform=EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);

	HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("hand_l")));
	HandSceneComponent->SetWorldTransform(ClipTransform);

	EquippedWeapon->SetMovingClip(true);

}

void ASpaceRogueCharacter::ReleaseClip()
{
	EquippedWeapon->SetMovingClip(false);
}

void ASpaceRogueCharacter::InitializeInterpLocations()
{
	FInterpLocation WeaponLocation{ WeaponInterpComp,0 };
	InterpLocations.Add(WeaponLocation);

	FInterpLocation InterpLoc1{ InterpComp1,0 };
	InterpLocations.Add(InterpLoc1);

	FInterpLocation InterpLoc2{ InterpComp2,0 };
	InterpLocations.Add(InterpLoc2);

	FInterpLocation InterpLoc3{ InterpComp3,0 };
	InterpLocations.Add(InterpLoc3);

	FInterpLocation InterpLoc4{ InterpComp4,0 };
	InterpLocations.Add(InterpLoc4);

	FInterpLocation InterpLoc5{ InterpComp5,0 };
	InterpLocations.Add(InterpLoc5);

	FInterpLocation InterpLoc6{ InterpComp6,0 };
	InterpLocations.Add(InterpLoc6);

}

int32 ASpaceRogueCharacter::GetInterpLocationIndex()
{
	int32 LowestIndex = 1;
	int32 LowestCount = INT_MAX;
	for (int32 i = 1; i < InterpLocations.Num(); i++)
	{
		if (InterpLocations[i].ItemCount < LowestCount)
		{
			LowestIndex = i;
			LowestCount = InterpLocations[i].ItemCount;
		}
	}
	return LowestIndex;
}

void ASpaceRogueCharacter::IncrementInterpLocItemCount(int32 Index, int32 Amount)
{
	if (Amount < -1 || Amount>1) return;
	if (InterpLocations.Num() >= Index)
	{
		InterpLocations[Index].ItemCount += Amount;
	}
}

void ASpaceRogueCharacter::StartPickupSoundTimer()
{
	bShouldPlayPickupSound = false;
	GetWorldTimerManager().SetTimer(PickupSoundTimer,this,&ASpaceRogueCharacter::ResetPickupSoundTimer,PickupSoundResetTime);
}

void ASpaceRogueCharacter::StartEquipSoundTimer()
{
	bShouldPlayEquipSound = false;
	GetWorldTimerManager().SetTimer(PickupSoundTimer, this, &ASpaceRogueCharacter::ResetEquipSoundTimer, EquipSoundResetTime);
}

// Called every frame
void ASpaceRogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraInterpZoom(DeltaTime);
	SetLookRates();
	CalculateCrosshairSpread(DeltaTime);

	TraceForItems();

}

// Called to bind functionality to input
void ASpaceRogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpaceRogueCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceRogueCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &ASpaceRogueCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASpaceRogueCharacter::LookUpAtRate);

	PlayerInputComponent->BindAxis("Turn", this, &ASpaceRogueCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASpaceRogueCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &ASpaceRogueCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("FireButton", IE_Released, this, &ASpaceRogueCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction("AimingButton", IE_Pressed, this, &ASpaceRogueCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", IE_Released, this, &ASpaceRogueCharacter::AimingButtonReleased);
	
	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &ASpaceRogueCharacter::SelectButtonPressed);
	PlayerInputComponent->BindAction("Select", IE_Released, this, &ASpaceRogueCharacter::SelectButtonReleased);

	PlayerInputComponent->BindAction("ReloadButton", IE_Pressed, this, &ASpaceRogueCharacter::ReloadButtonPressed);
}

float ASpaceRogueCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}

void ASpaceRogueCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if (OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bShouldTraceForItems = true;
	}
}

//FVector ASpaceRogueCharacter::GetCameraInterpLocation()
//{
//	const FVector CameraWorldLocation{ FollowCamera->GetComponentLocation() };
//	const FVector CameraForward{FollowCamera->GetForwardVector()};
	//desired = cameraWorldLocation + forward * A + Up * b
//	return CameraWorldLocation + CameraForward * CameraInt//erpDistance+FVector(0.f,0.f,CameraInterpElevation);
//}

void ASpaceRogueCharacter::GetPickupItem(AItem* Item)
{
	Item->PlayEquipSound();

	auto Weapon = Cast<AWeapon>(Item);
	if (Weapon)
	{
		SwapWeapon(Weapon);
	}
	auto Ammo = Cast<AAmmo>(Item);
	if (Ammo)
	{
		PickUpAmmo(Ammo);
	}
}

FInterpLocation ASpaceRogueCharacter::GetInterpLocation(int32 Index)
{
	if (Index <= InterpLocations.Num())
	{
		return InterpLocations[Index];
	}
	return FInterpLocation();
}

void ASpaceRogueCharacter::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0,Rotation.Yaw,0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);

	}
}
void ASpaceRogueCharacter::MoveRight(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0,Rotation.Yaw,0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void ASpaceRogueCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void ASpaceRogueCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASpaceRogueCharacter::FireWeapon()
{
	if (EquippedWeapon == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if (WeaponHasAmmo())
	{
		PlayFireSound();
		SendBullet();
		PlayGunfireMontage();
		EquippedWeapon->DecrementAmmo();
		StartFireTimer();
	}
}

void ASpaceRogueCharacter::TraceForItems()
{
	if (bShouldTraceForItems)
	{
		FHitResult ItemTraceResult;
		FVector HitLocation;
		TraceUnderCrosshairs(ItemTraceResult, HitLocation);

		if (ItemTraceResult.bBlockingHit)
		{
			TraceHitItem = Cast<AItem>(ItemTraceResult.Actor);
			if (TraceHitItem && TraceHitItem->GetPickupWidget())
			{
				TraceHitItem->GetPickupWidget()->SetVisibility(true);
				TraceHitItem->EnableCustomDepth();
			}
			if (TraceHitItemLastFrame)
			{
				if (TraceHitItem != TraceHitItemLastFrame)
				{
					TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
					TraceHitItemLastFrame->DisableCustomDepth();
				}
			}
			TraceHitItemLastFrame = TraceHitItem;
		}
	}
	else if (TraceHitItemLastFrame)
	{
		TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
		TraceHitItemLastFrame->DisableCustomDepth();
	}
}

void ASpaceRogueCharacter::AimingButtonPressed()
{
	bAiming = true;
	
	UE_LOG(LogTemp, Warning, TEXT("AIM!!!!!!!!!!!!"));
}

void ASpaceRogueCharacter::AimingButtonReleased()
{
	bAiming = false;

}

void ASpaceRogueCharacter::CameraInterpZoom(float DeltaTime)
{
	if (bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);
}

void ASpaceRogueCharacter::SetLookRates()
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void ASpaceRogueCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{ 0.f,600.f };
	FVector2D VelocityMultiplierRange{ 0.f,1.f };
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.f;

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(
		WalkSpeedRange, 
		VelocityMultiplierRange, 
		Velocity.Size());

	if (GetCharacterMovement()->IsFalling())
	{
		
		//spread the crosshairs slowly while in air
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			2.25f,
			DeltaTime,
			2.25f);
	}
	else
	{
		//shrink crosshairs quickly on ground
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			0.f,
			DeltaTime,
			30.f);
	}

	if (bAiming)
	{
		
		//shrink crosshairs a small amount very quickly
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor,
			0.6f,
			DeltaTime,
			30.f);
	}
	else
	{
		//spread crosshairs back to normal very quickly
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor,
			0.f,
			DeltaTime,
			30.f);
	}
	if (bFiringBullet)
	{
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor,
			0.3f, 
			DeltaTime, 
			60.f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor,
			0.f,
			DeltaTime,
			60.f);
	}
	CrosshairSpreadMultiplier =
		0.5f +
		CrosshairVelocityFactor +
		CrosshairInAirFactor-
		CrosshairAimFactor+
		CrosshairShootingFactor;
}

bool ASpaceRogueCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult,FVector& OutHitLocation)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	//get world position and direction of crosshairs
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * 50'000.f };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
			OutHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);
		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void ASpaceRogueCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;
	GetWorldTimerManager().SetTimer(
		CrosshairShootTimer,
		this,
		&ASpaceRogueCharacter::FinishCrosshairBulletFire,
		ShootTimeDuration);
}

void ASpaceRogueCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}

void ASpaceRogueCharacter::Turn(float Value)
{
	float TurnScaleFactor{};
	if (bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(Value * TurnScaleFactor);
}

void ASpaceRogueCharacter::LookUp(float Value)
{
	float LookUpScaleFactor{};
	if (bAiming)
	{
		LookUpScaleFactor = MouseAimingLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	AddControllerPitchInput(Value * LookUpScaleFactor);
}


