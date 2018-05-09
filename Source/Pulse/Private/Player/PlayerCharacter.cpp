// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Pulse.h"
#include "PulseController.h"
#include "Actors/Interactable.h"
#include "UnrealNetwork.h"
#include "Actors/Weapons/WeaponBase.h"
#include "GameModes/PulsePS.h"
#include "Blueprint/UserWidget.h"
#include "Actors/Grenade.h"
#include "Vehicles/VehicleBase.h"
#include "Actors/Weapons/SideArmsBase.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "GameModes/PulseGameMode.h"
#include "FootDecalActor.h"
#include "Actors/Weapons/SniperRifleBase.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	TPSCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSCameraBoom"));
	TPSCameraBoom->SetupAttachment(RootComponent);

	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepWorldTransform);

	ZoomInCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ZoomInCamera"));
	ZoomInCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepWorldTransform);
	ZoomInCamera->bAutoActivate = false;

	TPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamera"));
	TPSCamera->AttachToComponent(TPSCameraBoom, FAttachmentTransformRules::KeepWorldTransform);
	TPSCamera->bAutoActivate = false;
	
	ZoomInTPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ZoomInTPSCamera"));
	ZoomInTPSCamera->AttachToComponent(TPSCameraBoom, FAttachmentTransformRules::KeepWorldTransform);
	ZoomInTPSCamera->bAutoActivate = false;
	
	FPSMesh= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	//FPSMesh->AttachToComponent(FPSCamera, FAttachmentTransformRules::KeepWorldTransform);
	FPSMesh->AttachToComponent(FPSCamera,FAttachmentTransformRules::KeepWorldTransform);
	TPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPSMesh"));
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	
	PlayerNameRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerNameRender"));
	PlayerNameRender->SetupAttachment(RootComponent);

	//TPSMesh->AttachToComponent(TPSCamera, FAttachmentTransformRules::KeepWorldTransform);
	TPSMesh->SetupAttachment(RootComponent);
	
	TP_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TP_Weapon"));
	TP_Weapon->SetOnlyOwnerSee(false);			
	TP_Weapon->bCastDynamicShadow = true;
	TP_Weapon->CastShadow = true;
	TP_Weapon->SetVisibility(false);
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsMovementEnabled = true;
	bIsJumping = false;
	bIsRunning = false;
	bReplicates = true;
	bIsCrouching = false;
	bInZoom = false;
	bInFPS = true;
	bOffset = false;
	bIsTakingDamage = false;
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	//Weapon Setup FPS
	WeaponAttachPointMapFPS.Add(EWeaponType::ET_AssaultRifle, FName("WeaponSocket"));
	WeaponAttachPointMapFPS.Add(EWeaponType::ET_GrenadeLauncher, FName("RocketLauncherSocket"));
	WeaponAttachPointMapFPS.Add(EWeaponType::ET_Shotgun, FName("ShotgunSocket"));
	WeaponAttachPointMapFPS.Add(EWeaponType::ET_LaserRifle, FName("WeaponSocket"));
	WeaponAttachPointMapFPS.Add(EWeaponType::ET_RocketLauncher, FName("RocketLauncherSocket"));
	WeaponAttachPointMapFPS.Add(EWeaponType::ET_Pistol, FName("PistolSocket"));
	WeaponAttachPointMapFPS.Add(EWeaponType::ET_SniperRifle, FName("SniperRifleSocket"));
	//Weapon Setup TPS
	WeaponAttachPointMapTPS.Add(EWeaponType::ET_AssaultRifle, FName("WeaponSocket"));
	WeaponAttachPointMapTPS.Add(EWeaponType::ET_GrenadeLauncher, FName("RocketLauncherSocket"));
	WeaponAttachPointMapTPS.Add(EWeaponType::ET_Shotgun, FName("ShotgunSocket"));
	WeaponAttachPointMapTPS.Add(EWeaponType::ET_LaserRifle, FName("WeaponSocket"));
	WeaponAttachPointMapTPS.Add(EWeaponType::ET_RocketLauncher, FName("RocketLauncherSocket"));
	WeaponAttachPointMapTPS.Add(EWeaponType::ET_Pistol, FName("PistolSocket"));
	WeaponAttachPointMapTPS.Add(EWeaponType::ET_SniperRifle, FName("SniperRifleSocket"));

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->bAutoActivate = false;
	AudioComp->SetupAttachment(RootComponent);
}


void APlayerCharacter::SetPlayerTurret(ATurret * NewPlayerTurret)
{
	PlayerTurret = NewPlayerTurret;
}

void APlayerCharacter::ActivateWidgets()
{
	if (WaveHUD)
		WaveHUD->AddToViewport();
	if (PlayerHUD)
		PlayerHUD->AddToViewport();
	
}

void APlayerCharacter::DeactiveWidgets()
{
	if (WaveHUD)
		WaveHUD->RemoveFromParent();
	if (PlayerHUD)
		PlayerHUD->RemoveFromParent();
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	CurrentShield = MaxShield;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	TP_Weapon->AttachToComponent(TPSMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TPSWeaponSocketName);
	//Cast<APulsePS>(PlayerState)->LoadVariables()a
	if (WaveHUDClass)
	{
		WaveHUD = CreateWidget<UUserWidget>(GetWorld(), WaveHUDClass);
		WaveHUD->AddToViewport();
	}
	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		PlayerHUD->AddToViewport();
	}
	if (CloseCombatWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		CloseCombatWeapon = GetWorld()->SpawnActor<ASideArmsBase>(CloseCombatWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		CloseCombatWeapon->SetActorHiddenInGame(true);
		CloseCombatWeapon->AttachToComponent(TPSMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, CloseCombatSocketName);
		CloseCombatWeapon->SetOwner(this);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsRunning)
	{
		CurrentStamina+=-DeltaTime*0.20f*MaxStamina;
		if (CurrentStamina <= 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			bIsRunning = false;
		}

	}
	else
	{
		if (CurrentStamina <= MaxStamina)
			CurrentStamina+=DeltaTime*0.05f*MaxStamina;
	}
	if (Controller && Controller->IsLocalController())
	{
		AInteractable* Usable = FindFocusedActor();
		ATurret* UsableTurret = FindFocusedTurret();
		// End Focus
		if (FocusedActor != Usable)
		{
			if (FocusedActor)
			{
				FocusedActor->OnEndFocus();
			}

			bHasNewFocus = true;
		}
		if (FocusedTurret != UsableTurret)
		{
			if (FocusedTurret)
			{
				FocusedTurret->OnEndFocus();
			}
			bTurretHasNewFocus = true;
		}
		FocusedTurret = UsableTurret;
		// Assign new Focus
		FocusedActor = Usable;
		// Start Focus.
		if (Usable)
		{
			if (bHasNewFocus)
			{
				Usable->OnBeginFocus();
				bHasNewFocus = false;
			}
		}
		if (UsableTurret)
		{
			if (bTurretHasNewFocus)
			{
				UsableTurret->OnBeginFocus();
				bTurretHasNewFocus = false;
			}
		}
	}
	HandleActorRotation(DeltaTime);
}

void APlayerCharacter::AddBossUI(UUserWidget * NewBossUI)
{
	if (BossUI)
	{
		BossUI->RemoveFromParent();
	}
	BossUI = NewBossUI;
	BossUI->AddToViewport();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void APlayerCharacter::HandleHighlight()
{
	AInteractable* NewHighlight = FindFocusedActor();

	if (NewHighlight)
	{
		if (NewHighlight != FocusedActor) {
			if (FocusedActor) FocusedActor->OnEndFocus();
			NewHighlight->OnBeginFocus();
			FocusedActor = NewHighlight;
		}
	}
	else {
		if (FocusedActor) FocusedActor->OnEndFocus();
		FocusedActor = nullptr;
	}

}
void APlayerCharacter::Heal(float Value)
{
	if (CurrentHealth + Value <= MaxHealth)
		CurrentHealth += Value;
	else
		CurrentHealth = MaxHealth;
}
void APlayerCharacter::EquipWeapon(AWeaponBase * WeaponToEquip, int32 Slot)
{
	if (CurrentWeapon)
		CurrentWeapon->SetActorHiddenInGame(true);


	
	
	//TP_Weapon->DetachFromParent();
	//TP_Weapon->AttachToComponent(TPSMesh, FAttachmentTransformRules::KeepWorldTransform, WeaponAttachPointMap[CurrentWeapon->WeaponType]);
	CurrentWeapon = WeaponToEquip;
	ChangeTP_WeaponMesh();
	CurrentWeapon->AttachToComponent(FPSMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachPointMapFPS[CurrentWeapon->WeaponType]);
	CurrentWeapon->GetMesh()->SetOnlyOwnerSee(false);
	CurrentWeapon->GetMesh()->bCastDynamicShadow = false;
	CurrentWeapon->GetMesh()->CastShadow = false;
	CurrentSlot = Slot;
	CurrentWeapon->SetActorHiddenInGame(false);

}

void APlayerCharacter::AddToInventory(AWeaponBase * NewWeapon,int32 Slot)
{
	
	
		if (Slot <= 3)
		{
			bHasGun = true;
			FBackpackItem NewItem;
			NewItem.Weapon = NewWeapon;
			NewWeapon->SetActorEnableCollision(false);
			NewWeapon->ChangeOwner(this);
			NewWeapon->SetOwner(this);
			ChangeWeaponAttachment(NewWeapon);
			NewWeapon->SetActorHiddenInGame(true);
			
			NewItem.Slot = Slot;
			if (Backpack.Num() - 1 >= Slot)
			{
				if (Backpack[Slot - 1].Weapon != nullptr)
				{
					Backpack[Slot - 1] = NewItem;
				}
				else
				{
					Backpack.AddUnique(NewItem);
				}
			}
			else
			{
				Backpack.AddUnique(NewItem);
			}
			EquipWeapon(NewItem.Weapon, NewItem.Slot);
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Bad Slot"));
		}
	

}

int32 APlayerCharacter::SetWeaponSlot(AWeaponBase * Weapon)
{
	return Backpack.Num()+1;
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	bIsTakingDamage = true;
	float ActualDamage = 0.f;
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) {
		const FRadialDamageEvent& RadDmg = *(FRadialDamageEvent*)&DamageEvent;

		float MinDamage = RadDmg.Params.MinimumDamage;
		float MaxDamage = RadDmg.Params.BaseDamage;
		float Distance = (GetActorLocation() - RadDmg.Origin).Size();
		float LerpConst = Distance / RadDmg.Params.OuterRadius;
		ActualDamage = FMath::Lerp(MaxDamage, MinDamage, LerpConst);
	}
	else {
		ActualDamage = DamageAmount;
	}
	if (CurrentShield - ActualDamage < 0)
	{
		CurrentHealth = CurrentHealth+ (CurrentShield - ActualDamage);
		CurrentShield = 0;
	}
	else
	{
		CurrentShield -= ActualDamage;
	}
	bIsTakingDamage = false;
	if (CurrentHealth <= 0) {
		OnDeath();
	}
	AudioComp->SetSound(DamageTakenCue);
	AudioComp->Play();
	return ActualDamage;
}

void APlayerCharacter::OnDeath()
{
	DeathUI = CreateWidget<UUserWidget>(GetWorld(), DeathUIClass);
	DeathUI->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("You have died!"));
	WaveHUD->RemoveFromParent();
	PlayerHUD->RemoveFromParent();
	if (BossUI)
		BossUI->RemoveFromParent();
	Destroy();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(APlayerCharacter,FPSWeaponSocketName);
	//DOREPLIFETIME(APlayerCharacter, TPSWeaponSocketName);
	//DOREPLIFETIME(APlayerCharacter, PistolFPSWeaponSocketName);
	//DOREPLIFETIME(APlayerCharacter, PistolTPSWeaponSocketName);
	DOREPLIFETIME(APlayerCharacter, CurrentWeapon);
	DOREPLIFETIME(APlayerCharacter, bIsRunning);
	DOREPLIFETIME(APlayerCharacter, bIsCrouching);
	DOREPLIFETIME(APlayerCharacter, bHasGun);
	DOREPLIFETIME(APlayerCharacter, CurrentHealth);
	DOREPLIFETIME(APlayerCharacter, CurrentStamina);
	DOREPLIFETIME(APlayerCharacter, CurrentShield);
}
void APlayerCharacter::AddAmmo(EAmmoType AmmoType, float Amount)
{
	for (FBackpackItem Weapon : Backpack)
	{
		if (Weapon.Weapon->AmmoType == AmmoType)
		{
			Weapon.Weapon->AddAmmo(Amount);
			break;
		}
	}
}
void APlayerCharacter::ChangeWeaponAttachment(AWeaponBase* Weapon)
{
	FActorSpawnParameters SpawnParams;
	if (bHasGun)
	{
		if (bInFPS)
		{
		
			TP_Weapon->SetVisibility(false);
			Weapon->SetActorHiddenInGame(false);
			
			
		}
		else
		{
			TP_Weapon->SetVisibility(true);
			Weapon->SetActorHiddenInGame(true);
		}
	}
}
void APlayerCharacter::HandleActorRotation(float DeltaTime)
{
	FRotator AR{ 0,GetActorRotation().Yaw,0 };
	FRotator CR{ 0,GetControlRotation().Yaw,0 };
	float Delta = (CR - AR).Yaw;
	
	if(FMath::Abs(Delta)>=70 || bOffset)
	{
		bOffset = true;
		AddActorWorldRotation(FMath::RInterpTo({ 0,0,0 }, CR - AR, DeltaTime, 5));
		bOffset = !FMath::IsNearlyEqual(Delta, 0.0f, 2.0f);
		
	}
}
void APlayerCharacter::UpdateHUD()
{
	BP_UpdateHUD();
}
void APlayerCharacter::SetFov(float NewFov)
{
	TInlineComponentArray<UCameraComponent*> Cameras;
	GetComponents<UCameraComponent>(/*out*/ Cameras);
	for (UCameraComponent* CameraComponent : Cameras)
	{
		if (CameraComponent->bIsActive)
		{
			CameraComponent->SetFieldOfView(NewFov);
		}
	}
}
void APlayerCharacter::ThrowGrenade()
{
	
	if (GrenadeClass&&GrenadeAmount>0)
	{
		FActorSpawnParameters SpawnParams;
		AGrenade* Grenade=GetWorld()->SpawnActor<AGrenade>(GrenadeClass, TPSMesh->GetSocketLocation(GrenadeSocketName)+GetActorForwardVector()*ThrowStrength, TPSMesh->GetSocketRotation(GrenadeSocketName), SpawnParams);
		Grenade->SetOwningCharacter(this);
		Grenade->SetOwner(this);
		GrenadeAmount--;
	}
	
}
void APlayerCharacter::PlayReloadAnim()
{
	if (WeaponReloadAnimMap[CurrentWeapon->WeaponType])
	{
		FPSMesh->GetAnimInstance()->Montage_Play((WeaponReloadAnimMap[CurrentWeapon->WeaponType]));
		TPSMesh->GetAnimInstance()->Montage_Play((TPPWeaponReloadAnimMap[CurrentWeapon->WeaponType]));
	}
	if (ReloadCue)
	{
		AudioComp->SetSound(ReloadCue);
		AudioComp->Play();
	}
}
void APlayerCharacter::ChangeTP_WeaponMesh()
{
	TP_Weapon->SetSkeletalMesh(CurrentWeapon->GetMesh()->SkeletalMesh);
	 
	TP_Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	TP_Weapon->AttachToComponent(TPSMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachPointMapTPS[CurrentWeapon->WeaponType]);
}

void APlayerCharacter::SetupStartingWeapons()
{

		
		
		for (TSubclassOf<AWeaponBase> WeaponClass : StartingWeapons)
		{
			UE_LOG(LogTemp, Warning, TEXT("statring weapon not null"));
			FActorSpawnParameters SpawnParams;
			AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		
				Weapon->OnInteract(this);
			
			
		}
		if (StartingWeapons.Num() == 0)
			UE_LOG(LogTemp, Warning, TEXT("statring weapon null"));
	

}

void APlayerCharacter::CloseCombatAttack()
{
	if (bIsMeleeAttacking)
	{
		bChainAttack = true;
	}
	else
	{
		CloseCombatWeapon->SetActorHiddenInGame(false);
		CloseCombatWeapon->SetIsAttacking(true);
		HandleCombo();
	}
}
void APlayerCharacter::ChainCombo()
{
	if (bChainAttack)
	{
		bChainAttack = false;
		HandleCombo();
	}
}
void APlayerCharacter::ComboReset()
{
	bIsMeleeAttacking = false;
	bChainAttack = false;
	Combo = 1;
	CloseCombatWeapon->SetActorHiddenInGame(true);
	CloseCombatWeapon->SetIsAttacking(false);
}
void APlayerCharacter::HandleCombo()
{
	switch (Combo)
	{
	case 1:
		Combo++;
		TPSMesh->GetAnimInstance()->Montage_Play(AttackMontage);
		break;
	case 2:
		Combo++;
		TPSMesh->GetAnimInstance()->Montage_Play(SecondAttackMontage);
		break;
	case 3:
		TPSMesh->GetAnimInstance()->Montage_Play(ThirdAttackMontage);
		break;
	}
}
void APlayerCharacter::SimGrenade()
{
	FHitResult Hit(ForceInit);
	AGrenade* Grenade = Cast<AGrenade>(GrenadeClass->GetDefaultObject());
	FVector GrenadeVelocity = GetMovementComponent()->Velocity + (TPSMesh->GetSocketLocation(GrenadeSocketName).ForwardVector*Grenade->GetVelocity()) + (TPSMesh->GetSocketLocation(GrenadeSocketName).UpVector*Grenade->GetVelocity());
	FVector NextLocation = TPSMesh->GetSocketLocation(GrenadeSocketName) + GrenadeVelocity*10.0f;
	GetWorld()->LineTraceSingleByChannel(Hit, TPSMesh->GetSocketLocation(GrenadeSocketName), NextLocation, ECC_Weapon);
}
AInteractable * APlayerCharacter::FindFocusedActor()
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == nullptr)
		return nullptr;

	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * InteractionDistance);

	FCollisionQueryParams TraceParams(TEXT("TraceUsableActor"), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	/* Not tracing complex uses the rough collision instead making tiny objects easier to select. */
	TraceParams.bTraceComplex = false;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<AInteractable>(Hit.GetActor());
}
ATurret * APlayerCharacter::FindFocusedTurret()
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == nullptr)
		return nullptr;

	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * InteractionDistance);

	FCollisionQueryParams TraceParams(TEXT("TraceUsableActor"), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	/* Not tracing complex uses the rough collision instead making tiny objects easier to select. */
	TraceParams.bTraceComplex = false;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<ATurret>(Hit.GetActor());
}
void APlayerCharacter::UpdateCurrentHealth(float Amount)
{
	CurrentHealth += Amount;
}
void APlayerCharacter::UpdateCurrentShield(float Amount)
{
	if (CurrentShield + Amount <= MaxShield)
		CurrentShield += Amount;
	else
		CurrentShield = MaxShield;
	
}

void APlayerCharacter::UpdateGrenadeAmount(int Amount)
{
	GrenadeAmount += Amount;
}
void APlayerCharacter::ZoomInHUD()
{
	ASniperRifleBase* SniperWeapon = Cast<ASniperRifleBase>(CurrentWeapon);
	if (SniperWeapon)
	{
		if (SniperWeapon->ZoomInHUDClass)
		{
			ZoomInHUDRef = CreateWidget<UUserWidget>(GetWorld(), SniperWeapon->ZoomInHUDClass);
			ZoomInHUDRef->AddToViewport();
		}
		WaveHUD->RemoveFromParent();
		PlayerHUD->RemoveFromParent();
		CurrentWeapon->SetActorHiddenInGame(true);
	}

}
void APlayerCharacter::ZoomOutHUD()
{
	ASniperRifleBase* SniperWeapon = Cast<ASniperRifleBase>(CurrentWeapon);
	if (ZoomInHUDRef&&CurrentWeapon)
	{
		ZoomInHUDRef->RemoveFromParent();
		WaveHUD->AddToViewport();
		PlayerHUD->AddToViewport();
		CurrentWeapon->SetActorHiddenInGame(false);
	}
		
	
}
void APlayerCharacter::EnterQuitUI()
{
	if (ExitUIClass)
	{
		ExitUI = CreateWidget<UUserWidget>(GetWorld(), ExitUIClass);
		ExitUI->AddToViewport();
	}
	WaveHUD->RemoveFromParent();
	PlayerHUD->RemoveFromParent();
	if (BossUI)
		BossUI->RemoveFromParent();
	bIsInQuitUI = true;
}
void APlayerCharacter::ExitQuitUI()
{
	bIsInQuitUI = false;
	if (ExitUI)
		ExitUI->RemoveFromParent();
	WaveHUD->AddToViewport();
	PlayerHUD->AddToViewport();
	if (BossUI)
		BossUI->AddToViewport();
}