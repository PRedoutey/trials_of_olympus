// Fill out your copyright notice in the Description page of Project Settings.
#include "../Public/Enimes/BossMinotaur.h"

#include "DrawDebugHelpers.h"
#include "../Public/Enimes/BossMinotaurController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABossMinotaur::ABossMinotaur(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Health = 5000;
	MaxHealth = 5000;
	oldHealth = Health;
	const FString SkeletonPath = "/Game/Assets/SKM/SKM_Enemy_Minotaur_Chrzd_DP";
	GetMesh()->SetSkeletalMesh(Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *SkeletonPath)));
	GetMesh()->SetRelativeLocation(FVector(0.0,0.0,-80.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0,-230.0,0.0));
	GetMesh()->SetRelativeScale3D(FVector(1.0,1.0,1.0));
	
	GetCapsuleComponent()->SetCapsuleSize(65.0, 230.0);

	auto DeathAnimAsset = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Minotaur/BossMinotaurDieMontage2.BossMinotaurDieMontage2'"));
	if (DeathAnimAsset.Object != nullptr)
	{
		DeathAnim = DeathAnimAsset.Object;
	}
	auto const AttackAnimAsset2 = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Minotaur/BossMinotaurAttack2Montage2.BossMinotaurAttack2Montage2'"));
	if (AttackAnimAsset2.Object != nullptr)
	{
		AttackAnim2 = AttackAnimAsset2.Object;
	}
	auto const AttackAnimAsset3 = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Minotaur/BossMinotaurAttack3Montage2.BossMinotaurAttack3Montage2'"));
	if (AttackAnimAsset2.Object != nullptr)
	{
		AttackAnim3 = AttackAnimAsset3.Object;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABossMinotaurController::StaticClass();
}

void ABossMinotaur::Tick(float DeltaTime)
{
	if (GetCurrentMontage() != AttackAnim2 || GetCurrentMontage() != AttackAnim3 )
	{
		UStaticMeshComponent* H1 = static_cast<UStaticMeshComponent*>(GetDefaultSubobjectByName(TEXT("HammerHead")));
		if (H1 != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("COLLIDE NO MORE"));
			H1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		UStaticMeshComponent* H2 = static_cast<UStaticMeshComponent*>(GetDefaultSubobjectByName(TEXT("Hitbox 2")));
		if (H2 != nullptr)
		{
			H2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	Super::Tick(DeltaTime);
}

void ABossMinotaur::Die()
{
	Cast<ABaseEnemyController>(GetController())->BehaviorTreeComponent->StopTree();
	PlayAnimMontage(DeathAnim);
	Super::Die();
}