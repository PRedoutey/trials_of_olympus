// Copyright Epic Games, Inc. All Rights Reserved.

#include "../Public/Character/ProjectCCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "DrawDebugHelpers.h"
//#include "ModuleDescriptor.h"
#include "../Public/Enimes/RangedGolem.h"
//#include "../Public/Weapons/Gun.h"
//#include "../Public/Weapons/GunCore.h"
//#include "../Public/Weapons/StatusEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "UObject/Object.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "NiagaraComponent.h"
#include "Camera/PlayerCameraManager.h"

//what the fuck kind of include is this???
//#include "../../../../../../../Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.30.30705/include/vector"


#include "NiagaraComponentPool.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Enimes/SpawnZoneManagerActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AProjectCCharacter

AProjectCCharacter::AProjectCCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);
	
	FP_Core = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Core"));
	FP_Core->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Core->bCastDynamicShadow = false;
	FP_Core->CastShadow = false;
	// FP_Core->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Core->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	//FP_MuzzleLocation->SetupAttachment(FP_Gun);
	//FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	/** Sets base gun to the starter gun **/
	//AGun* starterGun = CreateDefaultSubobject<AGun>("starter gun", false);
	// AGun* starterGun = NewObject<AGun>();
	// gun1 = starterGun;
	// gun1->weaponMesh = pistolMesh;
	// currentGun = gun1;

	//core1 = NewObject<AGunCore>(this);
	//core2 = NewObject<AGunCore>(this);
	currentCore = core1;

	//vertRecoilPerShot = currentGun->StartingRecoil;

	// Set max health to 100
	MaxHealth = 100;
	// set health to max health
	Health = MaxHealth;

	BaseMoveSpeed = 600;
	Points = 0;
	
	canFire = true;
	isReloading = false;
	reloadTimer = 0;
	regenDelay = 10.0f;
	regenTimer = 0.0f;
	regenAmount = 2.0f;
	canRegen = true;
	isFiring = false;

	
	isDashing = false;
	dashCoolDownDuration = 1.5;
	curDashCoolDown = 0;
	dashTime = .1;
	dashEnded = true;
	curDashTime = 0;
	dashForce = 7500;
	
}

void AProjectCCharacter::BeginPlay()
{
	// Call the base class  
	AGun* starterGun = NewObject<AGun>(this);
	gun1 = starterGun;
	gun1->weaponMesh = pistolMesh;
	currentGun = gun1;
	vertRecoilPerShot = currentGun->StartingRecoil;
	starterGun->Destroy();
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	Mesh1P->SetHiddenInGame(false, true);
	//Mesh1P->SetAnimInstanceClass(currentGun->animBP->GeneratedClass);
	
	SwitchGun((1));

	
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnZoneManagerActor::StaticClass());
	MySpawnManager = Cast<ASpawnZoneManagerActor>(FoundActor);
	//MySpawnManager->Set_Player(this);

	//MySpawnManager = new(SpawnZoneManager);
	//MySpawnManager->Set_Player(this);
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectCCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump eventsda
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProjectCCharacter::OnJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AProjectCCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AProjectCCharacter::OnStopFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AProjectCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectCCharacter::MoveRight);

	// Bind dash event
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AProjectCCharacter::OnDash);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProjectCCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectCCharacter::LookUpAtRate);

	// Bind Switch Guns
	PlayerInputComponent->BindAxis("SwitchGun", this, &AProjectCCharacter::SwitchGun);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AProjectCCharacter::Reload);
}

void AProjectCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// regen
	if (regenTimer > 0)
	{
		regenTimer -= DeltaTime;
		if (regenTimer <= 0)
		{
			canRegen = true;
			regenTimer = 0;
		}
	}
	if (Health < MaxHealth && canRegen)
	{
	    ReceiveHealing();
		Health += regenAmount * DeltaTime;
		if (Health > MaxHealth)
		{
			Health = MaxHealth;
		}
	}
	
	//MySpawnManager->Update(DeltaTime);
	TArray<StatusEffect*> toDestroy;
	for (StatusEffect* affliction : Afflictions)
	{
		bool kill = affliction->Update(DeltaTime);
		if (kill)
		{
			toDestroy.Add(affliction);
		}
	}
	for (StatusEffect* affliction : toDestroy)
	{
		affliction->Die();
	}
	if(currentGun)
	{
		if (currentGun->curAmmo <= 0)
		{
			currentGun->curAmmo = 0;
			canFire = false;
			Reload();
		}
		if (canFire && isFiring && currentGun->fireCooldown <= 0)
		{
			OnFire();
			currentGun->fireCooldown = currentGun->fireDelay;
		}
	
		if (vertRecoilTotal < 0)
		{
			// do recoil
			AddControllerPitchInput(vertRecoilPerShot * DeltaTime);
			vertRecoilTotal -= vertRecoilPerShot;
		}
		if (isReloading)
		{
			reloadTimer -= DeltaTime;
			if (reloadTimer <= 0)
			{
				reloadTimer = 0;
				currentGun->curAmmo = currentGun->MaxAmmo;
				//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT("%d\n"), currentGun->curAmmo));
				canFire = true;
				isReloading = false;
				ReceiveFireReload();
			}
		}
	}
	if(isDashing == true)
	{
		if (!dashEnded)
		{
			// dashdamage
			if (currentCore && currentCore->CurrentElement == Electro && currentCore->PerkMap[5] == 1)
			{
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), FoundActors);
				for (AActor* actor : FoundActors)
				{
					ABaseEnemy* enemy = Cast<ABaseEnemy>(actor);
					// get location of enemy and AfflictedEnemy and check distance
					FVector tempLocation = enemy->GetActorLocation();
					float dist = FVector::Dist(GetActorLocation(), tempLocation);
					if (dist <= 500 && enemy->Health > 0)
					{
						enemy->Health -= 100 * currentCore->chainDamageModifier * DeltaTime;
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ARElectroParticle, GetActorLocation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), tempLocation), FVector(1,1,1), true, true);
					}
				}
			}
		}
		
		curDashCoolDown-=DeltaTime;
		curDashTime-=DeltaTime;
		if(curDashTime <= 0 && dashEnded == false)
		{
			//if(!GetCharacterMovement()->IsMovingOnGround())
			//{
				//GetMovementComponent()->GetLastInputVector().Normalize();
				//GetCharacterMovement()->AddImpulse(GetMovementComponent()->GetLastInputVector() * -dashForce, true);
				
			//}
			dashEnded = true;
		}
		if(curDashCoolDown <= 0)
		{
			isDashing = false;
		}
	}
	
}
// onShotgunFire not currently used
void AProjectCCharacter::onShotgunFire()
{
	UWorld* const World = GetWorld();
	FVector start = GetFirstPersonCameraComponent()->GetComponentLocation() + FVector(0.0f, 0.0f, 0.0f);
	FVector end = start + 5000.0 * GetFirstPersonCameraComponent()->GetForwardVector();
	
	
	// draw debug line
	if (currentCore != nullptr)
	{
		if (currentCore->CurrentElement == Element::Pyro)
		{
			//DrawDebugLine(World, start, end, FColor(255, 0, 0), false, .25, 0, 5.0);
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PyroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1), true, true);
			UNiagaraFunctionLibrary::SpawnSystemAttached(PyroMuzzle, FP_MuzzleLocation, "Muzzle", FVector::ZeroVector, FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true, true);
			UGameplayStatics::PlaySoundAtLocation(this, PyroSound, GetActorLocation());
		}
		else if (currentCore->CurrentElement == Element::Cryo)
		{
			//DrawDebugLine(World, start, end, FColor(0, 0, 255), false, .25, 0, 5.0);
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CryoParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1), true, true);
			UNiagaraFunctionLibrary::SpawnSystemAttached(CryoMuzzle, FP_MuzzleLocation, "Muzzle", FVector::ZeroVector, FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true, true);
			UGameplayStatics::PlaySoundAtLocation(this, CryoSound, GetActorLocation());
		}
		else if (currentCore->CurrentElement == Element::Electro)
		{
			//DrawDebugLine(World, start, end, FColor(255, 255, 0), false, .25, 0, 5.0);
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ElectroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1), true, true);
			UNiagaraFunctionLibrary::SpawnSystemAttached(ElectroMuzzle, FP_MuzzleLocation, "Muzzle", FVector::ZeroVector, FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true, true);
			UGameplayStatics::PlaySoundAtLocation(this, ElectroSound, GetActorLocation());
		}
	}
	else
	{
		//DrawDebugLine(World, start, end, FColor(255, 255, 255), false, .25, 0, 5.0);
		UNiagaraFunctionLibrary::SpawnSystemAttached(DefaultMuzzle, FP_MuzzleLocation, "Muzzle", FVector::ZeroVector, FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true, true);
	}
	
	// shoot hitscan trace
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	for(int i=0;i<=4;i++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Shotgun Fire\n")));
		FVector pelletOffset = FVector(FMath::RandRange(-500,500),FMath::RandRange(-500,500),FMath::RandRange(-500,500));
		isHit = World->LineTraceSingleByChannel(objHit, start, end + pelletOffset, ECollisionChannel::ECC_GameTraceChannel1, Params);
		//DrawDebugLine(World, start, end + pelletOffset, FColor(255, 255, 255), false, .25, 0, 5.0);
		if (currentCore != nullptr)
		{
			if (currentCore->CurrentElement == Element::Pyro)
			{
				//DrawDebugLine(World, start, end, FColor(255, 0, 0), false, .25, 0, 5.0);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SMGPyroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(UKismetMathLibrary::FindLookAtRotation(FP_MuzzleLocation->GetComponentLocation(), end + pelletOffset)), FVector(1,1,1), true, true);
			}
			else if (currentCore->CurrentElement == Element::Cryo)
			{
				//DrawDebugLine(World, start, end, FColor(0, 0, 255), false, .25, 0, 5.0);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SMGCryoParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(UKismetMathLibrary::FindLookAtRotation(FP_MuzzleLocation->GetComponentLocation(), end + pelletOffset)), FVector(1,1,1), true, true);
			}
			else if (currentCore->CurrentElement == Element::Electro)
			{
				//DrawDebugLine(World, start, end, FColor(255, 255, 0), false, .25, 0, 5.0);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SMGElectroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(UKismetMathLibrary::FindLookAtRotation(FP_MuzzleLocation->GetComponentLocation(), end + pelletOffset)), FVector(1,1,1), true, true);
			}
		}
		if(isHit)
		{
			if (currentCore == NULL)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DefaultHitspark, objHit.Location, FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1), true, true);
			}
			AActor* hitActor = objHit.GetActor();
			if(hitActor->GetClass()->IsChildOf(ABaseEnemy::StaticClass()))
			{
				// set hit enemy
				ABaseEnemy* hitEnemy = Cast<ABaseEnemy>(hitActor);
				// damage enemy
				float damageDelt;
				if (currentCore != nullptr)
				{
					damageDelt = currentGun->damage * currentCore->firedamageModifier;
					hitEnemy->Health -= damageDelt;
					// chance to freeze slowed enemies
					if (currentCore->CurrentElement == Cryo)
					{
						for (StatusEffect* affliction : Afflictions)
						{
							affliction->Freeze(hitEnemy, false);
						}
					}
				}
				else
				{
					damageDelt = currentGun->damage;
					hitEnemy->Health -= damageDelt;
				}
				/*if (currentCore != nullptr && ((currentCore->CurrentElement + 1) % 3 == hitEnemy->GetElement()))
				{
					hitEnemy->Health -= currentGun->damage * currentCore->damageModifier;
				}
				else
				{
					hitEnemy->Health -= currentGun->damage;
				}*/
				
				if (currentCore != nullptr)
				{
					int number = FMath::RandRange(0, 100);
					if (number <= currentCore->ChanceToAffect && !hitEnemy->IsAffected)
					{
						//currentCore->AffectedEnemies.Add(Cast<ABaseEnemy>(hitActor));
						//currentCore->OnApply();
						//StatusEffect* new_affliction = new StatusEffect();
						//new_affliction->Set_Core(currentCore);
						//new_affliction->Set_Enemy(hitEnemy);
						//new_affliction->Set_Player(this);
						//new_affliction->OnApply();
						//Afflictions.Add(new_affliction);
						//hitEnemy->IsAffected = true;
						currentCore->CreateAffliction(this, hitEnemy);
					}
				}
			}
		}
	}
	// decrement ammo
	currentGun->curAmmo--;
	ReceiveFireReload();

	// recoil
	vertRecoilPerShot += currentGun->RecoilDeminishingRate;
	if (vertRecoilPerShot >= currentGun->MinRecoilPerShot)
	{
		vertRecoilPerShot = currentGun->MinRecoilPerShot;
	}
	vertRecoilTotal += vertRecoilPerShot;
	
	// Makes guns deal damage
	
	
}

void AProjectCCharacter::OnFire()
{
	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	if (AnimInstance != nullptr && RecoilShake != NULL)
	{
		AnimInstance->Montage_Play(currentGun->fireAnimation[0], 1.0f);
		// recoil
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(RecoilShake, 1.0f);
		vertRecoilPerShot += currentGun->RecoilDeminishingRate;
		if (vertRecoilPerShot >= currentGun->MinRecoilPerShot)
		{
			vertRecoilPerShot = currentGun->MinRecoilPerShot;
		}
		vertRecoilTotal += vertRecoilPerShot;
	}
	UWorld* const World = GetWorld();
	FVector start = GetFirstPersonCameraComponent()->GetComponentLocation() + FVector(0.0f, 0.0f, 0.0f);
	FVector end = start + 5000.0 * GetFirstPersonCameraComponent()->GetForwardVector();
	
	float vfxScale = .5;
	if (currentCore != NULL)
	{
		if (currentCore->CurrentElement == Element::Pyro)
		{
			//DrawDebugLine(World, start, end, FColor(255, 0, 0), false, .25, 0, 5.0);
			if( currentGun->type == "Rifle")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ARPyroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			else if(currentGun->type == "Pistol")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PistolPyroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			else if(currentGun->type == "SMG")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SMGPyroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			else if(currentGun->type == "Shotgun")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShotgunPyroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			
			UNiagaraFunctionLibrary::SpawnSystemAttached(PyroMuzzle, FP_MuzzleLocation, "Muzzle", FVector::ZeroVector, FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true, true);
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PyroMuzzle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(.5, .5, .5), true, true);
			UGameplayStatics::PlaySoundAtLocation(this, PyroSound, GetActorLocation());
		}
		else if (currentCore->CurrentElement == Element::Cryo)
		{
			//DrawDebugLine(World, start, end, FColor(0, 0, 255), false, .25, 0, 5.0);
			if( currentGun->type == "Rifle")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ARCryoParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			else if(currentGun->type == "Pistol")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PistolCryoParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			else if(currentGun->type == "SMG")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SMGCryoParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			else if(currentGun->type == "Shotgun")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShotgunCryoParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			
			//UNiagaraFunctionLibrary::SpawnSystemAttached(CryoMuzzle, FP_MuzzleLocation, "Muzzle", FVector::ZeroVector, FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true, true);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CryoMuzzle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1, 1, 1), true, true);
			UGameplayStatics::PlaySoundAtLocation(this, CryoSound, GetActorLocation());
		}
		else if (currentCore->CurrentElement == Element::Electro)
		{
			//DrawDebugLine(World, start, end, FColor(255, 255, 0), false, .25, 0, 5.0);
			if( currentGun->type == "Rifle")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ARElectroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			else if(currentGun->type == "Pistol")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PistolElectroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			else if(currentGun->type == "SMG")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SMGElectroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			else if(currentGun->type == "Shotgun")
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShotgunElectroParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			
			//UNiagaraFunctionLibrary::SpawnSystemAttached(ElectroMuzzle, FP_MuzzleLocation, "Muzzle", FVector::ZeroVector, FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true, true);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ElectroMuzzle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1, 1, 1), true, true);
			UGameplayStatics::PlaySoundAtLocation(this, ElectroSound, GetActorLocation());
		}
	}
	else
	{
		//DrawDebugLine(World, start, end, FColor(255, 0, 0), false, .25, 0, 5.0);
		if( currentGun->type == "Rifle")
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ARDefaultParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
		else if(currentGun->type == "Pistol")
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PistolDefaultParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
		else if(currentGun->type == "SMG")
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SMGDefaultParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
		else if(currentGun->type == "Shotgun")
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShotgunDefaultParticle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1)*vfxScale, true, true);
			
		UNiagaraFunctionLibrary::SpawnSystemAttached(DefaultMuzzle, FP_MuzzleLocation, "Muzzle", FVector::ZeroVector, FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true, true);
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PyroMuzzle, FP_MuzzleLocation->GetComponentLocation(), FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(.5, .5, .5), true, true);
		UGameplayStatics::PlaySoundAtLocation(this, DefaultSound, GetActorLocation());
	}
	
	// shoot hitscan trace
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	float elementalDamageModifier = 1.0;
	if (currentGun->isShotgun)
	{
		for(int i=0;i<=4;i++)
		{
			FVector pelletOffset = FVector(FMath::RandRange(-500,500),FMath::RandRange(-500,500),FMath::RandRange(-500,500));
			isHit = World->LineTraceSingleByChannel(objHit, start, end + pelletOffset, ECollisionChannel::ECC_GameTraceChannel1, Params);
			if(isHit)
			{
				/*if (currentCore == NULL)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DefaultHitspark, objHit.Location, FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1), true, true);
				}*/
				AActor* hitActor = objHit.GetActor();
				if(hitActor->GetClass()->IsChildOf(ABaseEnemy::StaticClass()))
				{
					// set hit enemy
					ABaseEnemy* hitEnemy = Cast<ABaseEnemy>(hitActor);
					// damage enemy
					float damageDelt;
					if (currentCore != nullptr)
					{
						if (currentCore->CurrentElement == hitEnemy->GetElement())
						{
							elementalDamageModifier = 2.0;
						}
						damageDelt = currentGun->damage * currentCore->firedamageModifier * elementalDamageModifier;
						hitEnemy->Health -= damageDelt;
						// chance to freeze slowed enemies
						if (currentCore->CurrentElement == Cryo)
						{
							for (StatusEffect* affliction : Afflictions)
							{
								affliction->Freeze(hitEnemy, false);
							}
						}
					}
					else
					{
						damageDelt = currentGun->damage;
						hitEnemy->Health -= damageDelt;
					}
					if (currentCore != nullptr/* && currentCore->AffectTimer == 0*/)
					{
						int number = FMath::RandRange(0, 100);
						if (number <= currentCore->ChanceToAffect && !hitEnemy->IsAffected)
						{
							currentCore->CreateAffliction(this, hitEnemy);
						}
					}
				}
				ReceiveHitEnemy();
			}
		}
	}
	else
	{
		// reg gun
		isHit = World->LineTraceSingleByChannel(objHit, start, end, ECollisionChannel::ECC_GameTraceChannel1, Params);
		// Makes guns deal damage
		if(isHit)
		{
			/*if (currentCore == NULL)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DefaultHitspark, objHit.Location, FRotator(GetFirstPersonCameraComponent()->GetComponentRotation()), FVector(1,1,1), true, true);
			}*/
			AActor* hitActor = objHit.GetActor();
			if(hitActor->GetClass()->IsChildOf(ABaseEnemy::StaticClass()))
			{
				// set hit enemy
				ABaseEnemy* hitEnemy = Cast<ABaseEnemy>(hitActor);
				// damage enemy
				float damageDelt;
				if (currentCore != nullptr)
				{
					if (currentCore->CurrentElement == hitEnemy->GetElement())
					{
						elementalDamageModifier = 2.0;
					}
					damageDelt = currentGun->damage * currentCore->firedamageModifier * elementalDamageModifier;
					hitEnemy->Health -= damageDelt;
					// chance to freeze slowed enemies
					if (currentCore->CurrentElement == Cryo)
					{
						for (StatusEffect* affliction : Afflictions)
						{
							affliction->Freeze(hitEnemy, false);
						}
					}
				}
				else
				{
					damageDelt = currentGun->damage;
					hitEnemy->Health -= damageDelt;
				}
				/*
				if (currentCore != nullptr && ((currentCore->CurrentElement + 1) % 3 == hitEnemy->GetElement()))
				{
					damageDelt = currentGun->damage * currentCore->damageModifier * currentCore->damageOnIceModifier;
					hitEnemy->Health -= damageDelt;
				}
				else
				{
					damageDelt = currentGun->damage;
					hitEnemy->Health -= damageDelt;
				}
				*/
				if (currentCore != NULL/* && currentCore->AffectTimer == 0*/)
				{
					int number = FMath::RandRange(0, 100);
					if (number <= currentCore->ChanceToAffect && !hitEnemy->IsAffected)
					{
						currentCore->CreateAffliction(this, hitEnemy);
					}
				}
			}
			ReceiveHitEnemy();
		}
	}
	
	// decrement ammo
	currentGun->curAmmo--;
	ReceiveFireReload();
}

void AProjectCCharacter::OnStartFire()
{
	
	if(currentGun)
	{
		if (currentGun->isAutomatic)
		{
			isFiring = true;
		}
		else
		{
			if (canFire && currentGun->fireCooldown <= 0)
			{
				OnFire();
				currentGun->fireCooldown = currentGun->fireDelay;
			}
		}
	}
	
}

void AProjectCCharacter::OnStopFire()
{
	if(currentGun)
	{
		isFiring = false;
		vertRecoilPerShot = currentGun->StartingRecoil;
	}
	isHit = false;
}

void AProjectCCharacter::OnDash()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Got to dash loop\n")));
	if(isDashing == false && GetCharacterMovement()->IsMovingOnGround())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Got inside dash loop\n")));
		curDashCoolDown = dashCoolDownDuration;
		curDashTime = dashTime;
		dashEnded = false;
		//GetCharacterMovement()->AddImpulse(FVector(GetFirstPersonCameraComponent()->GetForwardVector().X, GetFirstPersonCameraComponent()->GetForwardVector().Y, 0) * dashForce, true);
		GetMovementComponent()->GetLastInputVector().Normalize();
		if (currentCore != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("%f\n"), currentCore->dashModifier));
			GetCharacterMovement()->AddImpulse(GetMovementComponent()->GetLastInputVector() * dashForce * currentCore->dashModifier, true);
		}
		else
		{
			GetCharacterMovement()->AddImpulse(GetMovementComponent()->GetLastInputVector() * dashForce, true);
		}
		isDashing = true;
		ReceiveDidDash();
	}
}

void AProjectCCharacter::OnJump()
{
	if (dashEnded)
	{
		ACharacter::Jump();
	}
}

void AProjectCCharacter::SwitchGun(float Val)
{
	if(currentGun)
	{
		switch((int)Val)
		{
		case 1:
			currentGun = gun1;
			currentCore = core1;
			canFire = (gun1->curAmmo) ? true : false;
		
			FP_Gun->SetSkeletalMesh(currentGun->weaponMesh);
			FP_MuzzleLocation->AttachToComponent(FP_Gun, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Muzzle");
			OnStopFire();
			isReloading = false;
			ReceiveSwapGun();
			ReceiveFireReload();
			if (currentCore)
			{
				Scale_Move_Speed(currentGun->speedModifier * currentCore->moveSpeedModifier);
			}
			else
			{
				Scale_Move_Speed(currentGun->speedModifier);
			}
			break;
		case -1:
			if (gun2 != nullptr)
			{
				currentGun = gun2;
				currentCore = core2;
				canFire = (gun2->curAmmo) ? true : false;

				FP_Gun->SetSkeletalMesh(currentGun->weaponMesh);
				FP_MuzzleLocation->AttachToComponent(FP_Gun, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Muzzle");
				OnStopFire();
				isReloading = false;
				ReceiveSwapGun();
				ReceiveFireReload();
				if (currentCore)
				{
					Scale_Move_Speed(currentGun->speedModifier * currentCore->moveSpeedModifier);
				}
				else
				{
					Scale_Move_Speed(currentGun->speedModifier);
				}
			}
			break;
			//case 0: break;
			default:
				break;
		};
	}
	
	if(currentGun->animBP != nullptr && Val!= 0)
	{
		Mesh1P->SetAnimInstanceClass(currentGun->animBP);
		FP_Core->AttachToComponent(FP_Gun, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("CoreSocket"));
		
		if(currentCore == nullptr)
			FP_Core->SetHiddenInGame(true);
		else
		{
			FP_Core->SetHiddenInGame(false);
			FP_Core->SetMaterial(0, currentCore->CoreMaterial);
		}
	}
	else if(Val != 0)
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("No Anim BP"));

	
	
	
}

void AProjectCCharacter::Reload()
{
	// tmp
	/*if (currentCore != nullptr)
	{
		currentCore->OnAcquire(1);
		currentCore->OnAcquire(2);
		currentCore->OnAcquire(3);
		currentCore->OnAcquire(4);
		currentCore->OnAcquire(5);
	}*/
	// tmp
	
	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	if(currentGun->reloadAnimation.Max() != 0)
	{
		if (!isReloading && currentGun->curAmmo < currentGun->MaxAmmo)
		{
			float tmpReloadSpeed;
			if (currentCore)
			{
				tmpReloadSpeed = currentGun->ReloadSpeed / currentCore->reloadSpeedModifer;
			}
			else
			{
				tmpReloadSpeed = currentGun->ReloadSpeed;
			}
			float scalar = currentGun->reloadAnimation[0]->SequenceLength / tmpReloadSpeed;
			//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("%f\n"), currentGun->reloadAnimation[0]->SequenceLength));
			AnimInstance->Montage_Play(currentGun->reloadAnimation[0], scalar / 1.5);
			canFire = false;
			isReloading = true;
			reloadTimer = tmpReloadSpeed;

			// slow or freeze nearby enemies
			if (currentCore && currentCore->CurrentElement == Cryo && currentCore->PerkMap[5] == 1)
			{
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), FoundActors);
				for (AActor* actor : FoundActors)
				{
					ABaseEnemy* enemy = Cast<ABaseEnemy>(actor);
					// get location of enemy and AfflictedEnemy and check distance
					FVector tempLocation = enemy->GetActorLocation();
					float dist = FVector::Dist(GetActorLocation(), tempLocation);
					if (dist <= 500 && enemy->Health > 0 && !enemy->IsAffected)
					{
						StatusEffect* new_affliction = currentCore->CreateAffliction(this, enemy);
						if (currentGun->curAmmo <= 0)
						{
							new_affliction->Freeze(enemy, true);
						}
					}
				}
			}
			
		}
	}
	
}

void AProjectCCharacter::Scale_Move_Speed(float scalar)
{
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed = BaseMoveSpeed * scalar;
}

void AProjectCCharacter::DamagePlayer(float damage)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT("hit\n")));
	if (currentCore)
	{
		Health -= damage * currentCore->damageIntakeModifier;
	}
	else
	{
		Health -= damage;
	}
	regenTimer = regenDelay;
	canRegen = false;
	ReceiveTakeDamage();
}

void AProjectCCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		//OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AProjectCCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AProjectCCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AProjectCCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AProjectCCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AProjectCCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProjectCCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AProjectCCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AProjectCCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AProjectCCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AProjectCCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}
