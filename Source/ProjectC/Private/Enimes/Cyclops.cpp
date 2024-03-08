// Fill out your copyright notice in the Description page of Project Settings.
#include "../Public/Enimes/Cyclops.h"

#include "DrawDebugHelpers.h"
#include "../Public/Enimes/CyclopsController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACyclops::ACyclops(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Health = 400;
    MaxHealth = 400;
	const FString SkeletonPath = "/Game/Assets/SKM/SKM_Enemy_Cyclops_Chrzd_DP";
	GetMesh()->SetSkeletalMesh(Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *SkeletonPath)));
	GetMesh()->SetRelativeLocation(FVector(0.0,0.0,-80.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0,-105.0,0.0));
	GetMesh()->SetRelativeScale3D(FVector(1.0,1.0,1.0));
	
	GetCapsuleComponent()->SetCapsuleSize(30.0, 105.0);

	auto const DeathAnimAsset = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Cyclops/CyclopsDieMontage.CyclopsDieMontage'"));
	if (DeathAnimAsset.Object != nullptr)
	{
		DeathAnim = DeathAnimAsset.Object;
	}
	auto const AttackAnimAsset = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Cyclops/CyclopsAttackMontage.CyclopsAttackMontage'"));
	if (AttackAnimAsset.Object != nullptr)
	{
		AttackAnim = AttackAnimAsset.Object;
	}
	auto const ThrowAnimAsset = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Cyclops/CyclopsThrowMontage.CyclopsThrowMontage'"));
	if (ThrowAnimAsset.Object != nullptr)
    {
    	ThrowAnim = ThrowAnimAsset.Object;
    }

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ACyclopsController::StaticClass();
	
	CurrencyDropped = 200;
}

void ACyclops::BeginPlay()
{
	Super::BeginPlay();
	SetRandomElement();
}

void ACyclops::Tick(float DeltaTime)
{
	if (GetCurrentMontage() != AttackAnim)
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
	if (GetCurrentMontage() != ThrowAnim)
	{
		/*UAnimMontage* mont = GetCurrentMontage();
		FString text;
		if (mont != nullptr)
		{
			text = mont->GetName(); 
		}
		else
		{
			text = "";
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, FString::Printf(TEXT("%s"), *text));*/
		UStaticMeshComponent* Prop = static_cast<UStaticMeshComponent*>(GetDefaultSubobjectByName(TEXT("Boulder Prop")));
		if (Prop != nullptr)
		{
			Prop->SetVisibility(false);
		}
	}
	Super::Tick(DeltaTime);
}

void ACyclops::Die()
{
	if (Cast<ABaseEnemyController>(GetController()))
	{
		Cast<ABaseEnemyController>(GetController())->BehaviorTreeComponent->StopTree();
	}
	PlayAnimMontage(DeathAnim, 1.0f, FName(TEXT("Start")));
	Super::Die();
}

// This is the C++ code required to call a blueprint function in C++, kept for posterity.
/*void ACyclops::PlayDeathAnim_Implementation()
{
}*/