// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/OutputDeviceNull.h"
#include "GameFramework/Character.h"
#include "Environment/Generator.h"
#include "BaseEnemy.generated.h" 

// May want to move to a util file in the future
// Enum of all of the elements currently possible for cores
UENUM(BlueprintType)
enum Element {Pyro, Cryo, Electro, None };


UCLASS()
class PROJECTC_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

protected:
	Element CurElement = None;
	
	

public:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterMovementComponent* CharacterMovementComponent;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0;

	UPROPERTY(BlueprintReadWrite)
	float oldHealth = 100.0;
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int damageTaken = 0;

	int CurrencyDropped = 100;
	
	bool IsAffected = false;
	
	bool IsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> GeneratorList;

	UPROPERTY( VisibleAnywhere)
	class UWidgetComponent* HealthWidgetComp;
	
	// Sets default values for this character's properties
	ABaseEnemy(const FObjectInitializer& ObjectInitializer);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Die();

	void SetRandomElement();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float& MoveSpeed();

	void DieHelp();

	UFUNCTION(BlueprintCallable)
	Element GetElement();
	
	UFUNCTION(BlueprintImplementableEvent)
	void gotHit(float damage);

	UFUNCTION(BlueprintCallable)
	void SetElement(Element newElement, bool skittle=false);
	

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
