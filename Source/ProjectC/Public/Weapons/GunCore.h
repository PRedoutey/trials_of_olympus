// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Public/Weapons/StatusEffect.h"
#include "../Enimes/BaseEnemy.h"
#include "GunCore.generated.h"


// MOVED TO BASEENEMY.H TO AVOID CIRCULAR INCLUDE LOOP
// Enum of all of the elements currently possible for cores
// UENUM(BlueprintType)
// enum Element { Pyro, Cryo, Electro };

class StatusEffect;
class AProjectCCharacter;
class ABaseEnemy;

UCLASS()
class PROJECTC_API AGunCore : public AActor
{
	GENERATED_BODY()
	
public:
	// Current element of the instance of a core
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float despawnTimer = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<Element> CurrentElement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* CoreMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int32, int32> PerkMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int> AquirablePerks;

	TArray<ABaseEnemy*> AffectedEnemies;
	
	float AffectDelay;
	float AffectTimer;
	
	// global modifiers
	int ChanceToAffect; // int out of 100
	float damageModifier;
	float damageIntakeModifier;

	// pyro modifiers
	float dotDamageModifier; //
	float dotTickModifier; //
	float firedamageModifier; //

	// cryo modifiers
	float slowingModifier; //
	// chance to freeze

	// electro modifiers
	float chainRangeModifier; // 
	float moveSpeedModifier;
	float reloadSpeedModifer;
	float chainDamageModifier; // 
	float dashModifier; //
	
	
	// Sets default values for this actor's properties
	AGunCore();

	UFUNCTION(BlueprintCallable)
	void SetElement(Element element);

	//void OnApply();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	int getType();

	UFUNCTION(BlueprintCallable)
	void OnAcquire(int perk);
	
	StatusEffect* CreateAffliction(AProjectCCharacter* player, ABaseEnemy* enemy);

	//UFUNCTION(BlueprintCallable)
	//void CreateFireAfflictionsBP(FVector location);

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnFirePool(FVector location);

};


AGunCore* CreateNewGunCore(Element element);
