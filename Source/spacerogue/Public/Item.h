// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName="Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),


	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditANywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditANywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditANywhere, BlueprintReadWrite)
	FLinearColor DarkColor;

	UPROPERTY(EditANywhere, BlueprintReadWrite)
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CustomDepthStencil;

};


UCLASS()
class SPACEROGUE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	/** sets the activestars array of bools based on rarity */
	void SetActiveStars();

	/**sets properties of the items components based on state */
	virtual void SetItemProperties(EItemState State);

	/** called when the interpTimer is finished */
	void FinishInterping();

	/** handles item interpolation when in the equip interping state */
	void ItemInterp(float DeltaTime);

	FVector GetInterpLocation();

	void PlayPickupSound(bool bForcePlaySound = false);
	
	
	virtual void InitializeCustomDepth();
	virtual void OnConstruction(const FTransform& Transform) override;

	void EnableGlowMaterial();

	void UpdatePulse();
	void ResetPulseTimer();
	void StartPulseTimer();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//called from spaceroguecharacter
	void PlayEquipSound(bool bForcePlaySound=false);
private:

	/*skeletal mesh for the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;


	/** line trace collides with box to show hud widgets */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Item Properties",meta=(AllowPrivateAccess="true"))
	class UBoxComponent* CollisionBox;

	/** popup widget for when the player looks at the item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;

	/** the curve asset to use for the items z location when iterping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZCurve;

	/** starting location when interping begins */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector ItemInterpStartLocation;

	/** target interp location in front of the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector CameraTargetLocation;

	/** true when interping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bInterping;

	/** plays when we start interping */
	FTimerHandle ItemInterpTimer;

	/** duration of the curve and timer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float ZCurveTime;

	/** pointer to the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class ASpaceRogueCharacter* Character;

	float ItemInterpX;
	float ItemInterpY;

	float InterpInitialYawOffset;
	
	/** curve used to scale item when interping */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemScaleCurve;

	/** sound played when item is picked up */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* PickupSound;

	/** sound played when item is equipped up */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 InterpLocIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 MaterialIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* MaterialInstance;

	bool bCanChangeCustomDepth;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveVector* PulseCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveVector* InterpPulseCurve;


	FTimerHandle PulseTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float PulseCurveTime;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float GlowAmount;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
		float FresnelExponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
		float FresnelReflectFraction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* AmmoItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	bool bCharacterInventoryFull;


	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=DataTable,meta=(AllowPrivateAccess="true"))
	class UDataTable* ItemRarityDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor GlowColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor LightColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor DarkColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	int32 NumberOfStars;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconBackground;

public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE EItemState GetItemState() const { return ItemState; }
	void SetItemState(EItemState State);
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }



	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound; }

	FORCEINLINE void SetPickupSound(USoundCue* Sound) { PickupSound = Sound; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }
	FORCEINLINE void SetEquipSound(USoundCue* Sound) { EquipSound = Sound; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex=Index; }
	FORCEINLINE void SetCharacter(ASpaceRogueCharacter* Char) { Character = Char; }
	FORCEINLINE void SetCharacterInventoryFull(bool bFull) { bCharacterInventoryFull = bFull; }
	FORCEINLINE void SetItemName(FString Name) { ItemName = Name; }
	FORCEINLINE UMaterialInstance* GetMaterialInstance() const { return MaterialInstance; }
	FORCEINLINE void SetMaterialInstance(UMaterialInstance* Instance) { MaterialInstance = Instance; }
	FORCEINLINE UMaterialInstanceDynamic* GetDynamicMaterialInstance() const { return DynamicMaterialInstance; }
	FORCEINLINE void SetDynamicMaterialInstance(UMaterialInstanceDynamic* Instance) {DynamicMaterialInstance = Instance; }
	FORCEINLINE FLinearColor GetGlowColor() const { return GlowColor; }
	FORCEINLINE int32 GetMaterialIndex() const {return MaterialIndex; }
	FORCEINLINE void SetMaterialIndex(int32 Index) { MaterialIndex = Index; }



	//icon for inventory
	FORCEINLINE void SetIconItem(UTexture2D* Icon) { IconItem = Icon; }
	//icon for pickupwidget
	FORCEINLINE void SetAmmoIcon(UTexture2D* Icon) { AmmoItem = Icon; }
	


	void StartItemCurve(ASpaceRogueCharacter* Char, bool bForcePlaySound = false);
	virtual void EnableCustomDepth();
	virtual void DisableCustomDepth();
	void DisableGlowMaterial();


};
