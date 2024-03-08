// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Enimes/BaseEnemy.h"
#include "../Public/Enimes/BaseEnemyController.h"
#include "Components/CapsuleComponent.h"
#include "../Public/Enimes/EnemyHealthBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Character/ProjectCCharacter.h"
#include "Math/Color.h"


// Sets default values
ABaseEnemy::ABaseEnemy(const FObjectInitializer& ObjectInitializer) : ACharacter( ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*(CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	CharacterMovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharacterMovementComponent"));*/

	HealthWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this,TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	UEnemyHealthBar* FloatingHealthBar = Cast<UEnemyHealthBar>( HealthWidgetComp->GetUserWidgetObject() );
	FloatingHealthBar->SetOwner(this);
		
	//FloatingHealthBar->SetForegroundColor(FSlateColor(FLinearColor(1.0, 1.0, 0.0, 1.0)));
	//FloatingHealthBar->SetColorAndOpacity(FLinearColor(1.0, 0.0, 0.0, 1.0));

	// maybe put this in a global class
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenerator::StaticClass(), GeneratorList);
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Health < oldHealth)
	{
		damageTaken = oldHealth - Health;
		oldHealth = Health;
		if (damageTaken > 0)
			gotHit(damageTaken);
		if (Health <= 0)
		{
			
			if (!IsDead)
			{
				Die();
			}
		}
	}

}

float& ABaseEnemy::MoveSpeed()
{
	return GetCharacterMovement()->MaxWalkSpeed;
}

void ABaseEnemy::Die()
{
	// generator code
	//FHitResult objHit;
	//bool isHit;
	TArray<AActor*> enemies;
	AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), AProjectCCharacter::StaticClass());

	// This is used to give the player points
	AProjectCCharacter* Player = Cast<AProjectCCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AProjectCCharacter::StaticClass()));
	float CurrencyScalar = FMath::RandRange(0.70f, 1.30f);
	Player->Points += CurrencyDropped * CurrencyScalar;
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("%d\n"), CurrencyDropped));
	for (AActor* actor : GeneratorList)
	{
		AGenerator* generator = Cast<AGenerator>(actor);
		float dist = FVector::Dist(GetActorLocation(), generator->GetActorLocation());
		if (dist <= generator->MaxDistance)
		{
			generator->AddCount();
			/*FCollisionQueryParams Params;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), enemies);
			Params.AddIgnoredActors(enemies);
			Params.AddIgnoredActor(player);
			isHit = GetWorld()->LineTraceSingleByChannel(objHit, GetActorLocation(), generator->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1, Params);
			DrawDebugLine(GetWorld(), GetActorLocation(), generator->GetActorLocation(), FColor(255, 255, 255), false, .25, 0, 5.0);
			if (isHit)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("True\n")));
				AActor* hitActor = objHit.GetActor();
				if(hitActor == generator->MyGenerator)
				{
					generator->AddCount();
				}
			}*/
		}
	}
	// end generator code

	// Heal player on death --- might change healing method in future
	/*if (player)
	{
		AProjectCCharacter* player_cast = Cast<AProjectCCharacter>(player);
		player_cast->Health += 5;
		if(player_cast->Health > player_cast->MaxHealth)
		{
			player_cast->Health = player_cast->MaxHealth;
		}
	}*/
	
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FTimerHandle MemberTimerHandle;
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ABaseEnemy::DieHelp, 1.0f, false, 30.0f);
	IsDead = true;
}

void ABaseEnemy::DieHelp()
{
	this->Destroy();
}

void ABaseEnemy::SetRandomElement()
{
	const int Number = FMath::RandRange(0, 2);
	Element newElement;
	switch(Number)
	{
	case 0:
		newElement = Element::Pyro;
		break;

	case 1:
		newElement = Element::Cryo;
		break;

	case 2:
		newElement = Element::Electro;
		break;

	// This case shouldn't ever be used
	default:
		newElement = Element::None;
		break;
	}
	SetElement(newElement, false);
}

Element ABaseEnemy::GetElement()
{
	return CurElement;
}


// skittle determines if all enemy materials will be overridden with element color (this is temporary until all enemy textures are available)
void ABaseEnemy::SetElement(Element newElement, bool skittle)
{
	UEnemyHealthBar* FloatingHealthBar = Cast<UEnemyHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	CurElement = newElement;
	FString MPath;
	switch(newElement)
	{
	case Pyro:
		CurElement = Element::Pyro;
		MPath = "/Game/FirstPerson/Enemies/Materials/RedTestMaterial";
		FloatingHealthBar->HealthBar->SetFillColorAndOpacity(FLinearColor(.9, .05, .11, 1.0));
		break;

	case Cryo:
		CurElement = Element::Cryo;
		MPath = "/Game/FirstPerson/Enemies/Materials/BlueTestMaterial";
		FloatingHealthBar->HealthBar->SetFillColorAndOpacity(FLinearColor(.32, .79, .76, 1.0));
		break;

	case Electro:
		CurElement = Element::Electro;
		MPath = "/Game/FirstPerson/Enemies/Materials/YellowTestMaterial";
		FloatingHealthBar->HealthBar->SetFillColorAndOpacity(FLinearColor(1.0, .89, .35, 1.0));
		break;

	default:
		MPath = "";
		break;
	}

	UMaterial* Mat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *MPath));
	// TODO: Figure out which material needs to be changed so they aren't Skittles, then replace this commented code
	if (skittle)
	{
		for (int32 i = 0; i < GetMesh()->GetNumMaterials(); i++)
			GetMesh()->SetMaterial(i, Mat);		
	}
}
