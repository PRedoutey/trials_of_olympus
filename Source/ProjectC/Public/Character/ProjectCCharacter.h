// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../Public/Weapons/Gun.h"
#include "../Public/Weapons/GunCore.h"
#include "../Public/Weapons/StatusEffect.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "NiagaraSystem.h"
//#include "Enimes/SpawnZoneManagerActor.h"
#include "MatineeCameraShake.h"
#include "ProjectCCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class ASpawnZoneManagerActor;

UCLASS(config=Game)
class AProjectCCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;
	
	/** Core mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Core;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AProjectCCharacter();

	/** Initializing player variables*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player_Stats)
	float Health;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Player_Stats)
	float MaxHealth;

	float BaseMoveSpeed;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Player_Stats)
	float Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* pistolMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* arMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* smgMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* shotgunMesh;
	

	/** Initializes gun and core variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	AGun* gun1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	AGun* gun2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	AGun* currentGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	AGunCore* core1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	AGunCore* core2;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly ,Category = Inventory)
	AGunCore* currentCore;


	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* DefaultHitspark;
	
	// Default tracers
	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* ARDefaultParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* PistolDefaultParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* SMGDefaultParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* ShotgunDefaultParticle;
	
	// Pyro tracers
	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* ARPyroParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* PistolPyroParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* SMGPyroParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* ShotgunPyroParticle;
	
	// Cryo tracers
	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* ARCryoParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* PistolCryoParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* SMGCryoParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* ShotgunCryoParticle;

	// Electro tracers
	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* ARElectroParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* PistolElectroParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* SMGElectroParticle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* ShotgunElectroParticle;

	
	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* DefaultMuzzle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* PyroMuzzle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* CryoMuzzle;

	UPROPERTY(EditAnywhere, Category = FX)
	UNiagaraSystem* ElectroMuzzle;

	UPROPERTY(EditAnywhere, Category = FX)
	USoundBase* DefaultSound;

	UPROPERTY(EditAnywhere, Category = FX)
	USoundBase* PyroSound;

	UPROPERTY(EditAnywhere, Category = FX)
	USoundBase* CryoSound;

	UPROPERTY(EditAnywhere, Category = FX)
	USoundBase* ElectroSound;

	UPROPERTY(EditAnywhere, Category = Managers)
	ASpawnZoneManagerActor* MySpawnManager;

	TArray<StatusEffect*> Afflictions;

	bool canFire;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool isReloading;
	float reloadTimer;
	float regenDelay;
	float regenTimer;
	float regenAmount;
	bool canRegen;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitDetection)
	FHitResult objHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitDetection)
	bool isHit;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool isFiring;
	
	//UI funcs
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "SwapGun"))
	void ReceiveSwapGun();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "FireReload"))
	void ReceiveFireReload();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TakeDamage"))
	void ReceiveTakeDamage();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "DidDash"))
	void ReceiveDidDash();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "HitEnemy"))
	void ReceiveHitEnemy();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnHeal"))
	void ReceiveHealing();
	
	bool isDashing; // Whether or not the player's dash is on cooldown
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player_Stats)
	float dashCoolDownDuration; // What the curDashCoolDown is reset to
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player_Stats)
	float curDashCoolDown; // Current time before the player can dash again
	
	float dashTime; // What the curDashTime is reset too
	bool dashEnded; // Whether or not the dash was ended
	float curDashTime; // How long until an opposite force is applied to the player, stopping the dash
	float dashForce; // Force used in the impulse of the dash
	
	
	float vertRecoilPerShot;
	float vertRecoilTotal;
	//float startingRecoilPerShot;
	//float minRecoilPerShot;
	//float recoilDeminishingRate;
	//float recoilHorizontalOffset;


	
protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> RecoilShake;

	/** Projectile class to spawn *

	/** Sound to play each time we fire */
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;*/

protected:

	void Tick(float DeltaTime);

	void onShotgunFire();
	
	/** Fires a projectile. */
	void OnFire();

	void OnStartFire();

	void OnStopFire();

    void OnDash();

	void OnJump();

	/** Switch to your other gun **/
	UFUNCTION(BlueprintCallable)
	void SwitchGun(float Val);

	void Reload();

	void Scale_Move_Speed(float scalar);

	UFUNCTION(BlueprintCallable)
	void DamagePlayer(float damage);

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	

};

