// Fill out your copyright notice in the Description page of Project Settings.
#include "../Public/Enimes/Soldier.h"

#include "DrawDebugHelpers.h"
#include "../Public/Enimes/SoldierController.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASoldier::ASoldier(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	const FString SkeletonPath = "/Game/Assets/SKM/SKM_Enemy_Soldier_Char_DP";
	GetMesh()->SetSkeletalMesh(Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *SkeletonPath)));
	GetMesh()->SetRelativeLocation(FVector(0.0,0.0,-160.0));
	//GetMesh()->SetRelativeRotation(FRotator(0.0,-230.0,0.0));
	GetMesh()->SetRelativeScale3D(FVector(1.0,1.0,1.0));

	//TODO: THIS
	GetCapsuleComponent()->SetCapsuleSize(25.0, 100.0);

	auto DeathAnimAsset = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Soldier/SoldierDieMontage.SoldierDieMontage'"));
	if (DeathAnimAsset.Object != nullptr)
	{
		DeathAnim = DeathAnimAsset.Object;
	}
	auto const AttackAnimAsset1 = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Soldier/SoldierAttackMontage.SoldierAttackMontage'"));
	if (AttackAnimAsset1.Object != nullptr)
	{
		AttackAnim1 = AttackAnimAsset1.Object;
	}
	auto const AttackAnimAsset2 = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Soldier/SoldierAttack2Montage.SoldierAttack2Montage'"));
	if (AttackAnimAsset2.Object != nullptr)
	{
		AttackAnim2 = AttackAnimAsset2.Object;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ASoldierController::StaticClass();
}

void ASoldier::Tick(float DeltaTime)
{
	if (GetCurrentMontage() != AttackAnim1)
	{
		UBoxComponent* H1 = static_cast<UBoxComponent*>(GetDefaultSubobjectByName(TEXT("Hitbox 1")));
		if (H1 != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("COLLIDE NO MORE"));
			H1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		UBoxComponent* H2 = static_cast<UBoxComponent*>(GetDefaultSubobjectByName(TEXT("Hitbox 2")));
		if (H2 != nullptr)
		{
			H2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	if (GetCurrentMontage() != AttackAnim2)
	{
	    EndTracking = false;
		EndMovement = false;
		UBoxComponent* H3 = static_cast<UBoxComponent*>(GetDefaultSubobjectByName(TEXT("Hitbox 3")));
		if (H3 != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("COLLIDE NO MORE 2"));
			H3->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		UBoxComponent* H4 = static_cast<UBoxComponent*>(GetDefaultSubobjectByName(TEXT("Hitbox 4")));
		if (H4 != nullptr)
		{
			H4->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	Super::Tick(DeltaTime);
}

void ASoldier::Die()
{
	//UE_LOG(LogTemp, Warning, TEXT("YES, I KNOW HOW TO DIE"));
	Cast<ABaseEnemyController>(GetController())->BehaviorTreeComponent->StopTree();
	PlayAnimMontage(DeathAnim);
	Super::Die();
}