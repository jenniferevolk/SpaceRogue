// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BulletHitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class SPACEROGUE_API AEnemy : public ACharacter, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** called when something overlaps with the agrosphere */

	UFUNCTION()
	void AgroSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/** particles to spawn when hit by bullets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta=(AllowPrivateAccess = "true"))

	class UParticleSystem* ImpactParticles;

	/** sound to play when hit by bullets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta=(AllowPrivateAccess = "true"))
	class USoundCue* ImpactSound;

	/** particles to spawn when dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))

		class UParticleSystem* DeathParticles;

	/** sound to play when dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class USoundCue* DeathSound;




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere,Category="Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	/** point for the enemy to move to */
	UPROPERTY(EditAnywhere,Category="Behavior Tree", meta = (AllowPrivateAccess = "true",MakeEditWidget="true"))
	FVector PatrolPoint;

	/** point for the enemy to move to */
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
		FVector PatrolPoint2;

	class AEnemyController* EnemyController;

	/** overlap sphere for when the enmy becomes hostile */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Combat,meta=(AllowPrivateAccess))
	class USphereComponent* AgroSphere;

	int8 numberOfHits;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult) override;

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }


};
