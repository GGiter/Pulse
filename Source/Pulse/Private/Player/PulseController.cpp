// Fill out your copyright notice in the Description page of Project Settings.

#include "PulseController.h"
#include "Pulse.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Actors/Interactable.h"
#include "Actors/Weapons/WeaponBase.h"
#include "UnrealNetwork.h"
#include "PlayerAnimInstance.h"
#include "Vehicles/VehicleBase.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Turret/Turret.h"
#include "Blueprint/UserWidget.h"

void APulseController::BeginPlay()
{
	Super::BeginPlay();
	
	
	
}

void APulseController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAxis("MoveX", this, &APulseController::MoveX);
		InputComponent->BindAxis("MoveY", this, &APulseController::MoveY);
		InputComponent->BindAxis("LookPitch", this, &APulseController::LookPitch);
		InputComponent->BindAxis("LookYaw", this, &APulseController::LookYaw);
		InputComponent->BindAction("Jump",IE_Pressed, this, &APulseController::Jump);
		InputComponent->BindAction("Jump", IE_Pressed, this, &APulseController::HandbrakeReleased);
		InputComponent->BindAction("Use", IE_Pressed, this, &APulseController::Use);
		InputComponent->BindAction("Fire", IE_Pressed, this, &APulseController::StartFiring);
		InputComponent->BindAction("Fire", IE_Released, this, &APulseController::StopFiring);
		InputComponent->BindAction("Sprint", IE_Pressed, this, &APulseController::SprintOn);
		InputComponent->BindAction("Sprint", IE_Released, this, &APulseController::SprintOff);
		InputComponent->BindAction("Crouch", IE_Pressed, this, &APulseController::CrouchToggle);
		InputComponent->BindAction("Reload", IE_Pressed, this, &APulseController::Reload);
		InputComponent->BindAction("Switch1", IE_Pressed, this, &APulseController::SwitchToFirstSlot);
		InputComponent->BindAction("Switch2", IE_Pressed, this, &APulseController::SwitchToSecondSlot);
		InputComponent->BindAction("Switch3", IE_Pressed, this, &APulseController::SwitchToThirdSlot);
		InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APulseController::ZoomIn);
		InputComponent->BindAction("ZoomIn", IE_Released, this, &APulseController::ZoomIn);
		InputComponent->BindAction("ChangeView", IE_Pressed, this, &APulseController::ChangeView);
		InputComponent->BindAction("Throw", IE_Pressed, this, &APulseController::Throw);
		InputComponent->BindAction("CloseCombatAttack", IE_Pressed, this, &APulseController::CloseCombatAttack);
		InputComponent->BindAction("Quit", IE_Pressed, this, &APulseController::QuitUI);
	}
}

void APulseController::MoveX(float Value)
{
	if (!PlayerRef&&!TurretRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		TurretRef = Cast<ATurret>(GetPawn());
		
	}
	else if(PlayerRef)
	{
		if ((PlayerRef->bIsMovementEnabled) && (Value != 0.0f))
		{
			const FRotator Rotation = ControlRotation;
			const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			Value = FMath::Clamp<float>(Value, -1.0f, 1.0f);
			PlayerRef->AddMovementInput(Direction, Value);
		}
	}
	else if (TurretRef)
	{
		TurretRef->MoveGuns(Value);
	}

}

void APulseController::MoveY(float Value)
{
	if (!PlayerRef && !TurretRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		TurretRef = Cast<ATurret>(GetPawn());
	}
	else if (PlayerRef)
	{
	if ((PlayerRef->bIsMovementEnabled) && (Value != 0.0f))
	{
		const FRotator Rotation = ControlRotation;
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		Value = FMath::Clamp<float>(Value, -1.0f, 1.0f);
		PlayerRef->AddMovementInput(Direction, Value);
	}
	}
	else if (TurretRef)
	{
		TurretRef->MoveMainBody(Value);
	}

}
void APulseController::LookPitch(float Value)
{
	if (!TurretRef)
	AddPitchInput(Value);
}
void APulseController::LookYaw(float Value)
{
	if (!TurretRef)
	AddYawInput(Value);
}
void APulseController::Jump_Implementation()
{

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
	
	}
	else if (PlayerRef)
	{
	if (PlayerRef->GetCharacterMovement()->IsMovingOnGround())
		{
			PlayerRef->bIsJumping = true;
		
			PlayerRef->GetCharacterMovement()->DoJump(true);
			
		}
	}

}
bool APulseController::Jump_Validate()
{
	return true;
}
void APulseController::Use_Implementation()
{
	if (!PlayerRef && !TurretRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		TurretRef = Cast<ATurret>(GetPawn());
	}
	else if (PlayerRef)
	{	
		AInteractable *Interactable = PlayerRef->FindFocusedActor();
		ATurret *InteractableTurret = PlayerRef->FindFocusedTurret();
		if (Interactable)
		{
			Interactable->OnInteract(PlayerRef);
		}
		if (InteractableTurret)
		{
			InteractableTurret->OnInteract(PlayerRef);

		}
	}
	else if (TurretRef)
	{
		TurretRef->DeactivateWidgets();
		PlayerRef = TurretRef->Player;
		FVector PlayerLocation{ TurretRef->GetActorLocation().X + 20,TurretRef->GetActorLocation().Y + 20,TurretRef->GetActorLocation().Z + 20 };
		PlayerRef->SetActorLocation(PlayerLocation);
		PlayerRef->SetActorHiddenInGame(false);
		PlayerRef->SetActorEnableCollision(true);
		UnPossess();
		Possess(PlayerRef);
		PlayerRef->ActivateWidgets();
		TurretRef = nullptr;
	}

}
bool APulseController::Use_Validate()
{
	return true;
}
void APulseController::StartFiring()
{
	if (!PlayerRef && !TurretRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		TurretRef = Cast<ATurret>(GetPawn());
	}
	else if (PlayerRef)
	{
		if (PlayerRef->CurrentWeapon)
		{
			
			PlayerRef->GetCurrentWeapon()->StartFire();
			PlayerRef->FPSMesh->GetAnimInstance()->Montage_Play(PlayerRef->WeaponFireAnimMap[PlayerRef->CurrentWeapon->WeaponType]);
			PlayerRef->TPSMesh->GetAnimInstance()->Montage_Play(PlayerRef->TPPWeaponFireAnimMap[PlayerRef->CurrentWeapon->WeaponType]);
			
		}
	}
	else if (TurretRef)
	{
		TurretRef->StartFire();
	}

}
void APulseController::StopFiring()
{

	if (!PlayerRef && !TurretRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		TurretRef = Cast<ATurret>(GetPawn());
	}
	else if (PlayerRef)
	{
		if (PlayerRef->GetCurrentWeapon())
			PlayerRef->GetCurrentWeapon()->StopFire();
	}
	else if (TurretRef)
	{
		TurretRef->StopFire();
	}

}
void APulseController::SprintOn_Implementation()
{
	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
	}
	else if (PlayerRef)
	{
		PlayerRef->bIsRunning = true;
		PlayerRef->GetCharacterMovement()->MaxWalkSpeed = PlayerRef->GetRunSpeed();
		PlayerRef->AudioComp->SetSound(PlayerRef->SprintCue);
		PlayerRef->AudioComp->Play();
	}

}
bool APulseController::SprintOn_Validate()
{
	return true;
}
void APulseController::SprintOff_Implementation()
{
	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		
	}
	else if (PlayerRef)
	{
		PlayerRef->bIsRunning = false;
		PlayerRef->GetCharacterMovement()->MaxWalkSpeed = PlayerRef->GetWalkSpeed();
	}

}
bool APulseController::SprintOff_Validate()
{
	return true;
}
void APulseController::CrouchToggle_Implementation()
{
	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		
	}
	else if (PlayerRef)
	{
		if (!PlayerRef->bIsCrouching)
		{
			
			PlayerRef->bIsCrouching = true;
		}
		else
		{
			
			PlayerRef->bIsCrouching = false;
		
		}
	}
}
bool APulseController::CrouchToggle_Validate()
{
	return true;
}
void APulseController::Reload()
{
	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		
	}
	else if (PlayerRef)
	{
		if (PlayerRef->GetCurrentWeapon())
		{
			PlayerRef->GetCurrentWeapon()->Reload();
			PlayerRef->PlayReloadAnim();
				
				
		}
	}
}
void APulseController::SwitchToFirstSlot()
{

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
	}
	else if (PlayerRef)
	{
		if (PlayerRef->Backpack.Num() >= 1)
		{
			PlayerRef->EquipWeapon(PlayerRef->Backpack[0].Weapon, 1);
			PlayerRef->AudioComp->SetSound(PlayerRef->ChangeWeaponCue);
			PlayerRef->AudioComp->Play();
		}
	
	}
}
void APulseController::SwitchToSecondSlot()
{

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
	}
	else if (PlayerRef)
	{
		if (PlayerRef->Backpack.Num() >= 2)
		{
			PlayerRef->EquipWeapon(PlayerRef->Backpack[1].Weapon, 2);
			PlayerRef->AudioComp->SetSound(PlayerRef->ChangeWeaponCue);
			PlayerRef->AudioComp->Play();
		}
	
	}
}
void APulseController::SwitchToThirdSlot()
{

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
	}
	else if (PlayerRef)
	{
		if (PlayerRef->Backpack.Num() >= 3)
		{
			PlayerRef->EquipWeapon(PlayerRef->Backpack[2].Weapon, 3);
			PlayerRef->AudioComp->SetSound(PlayerRef->ChangeWeaponCue);
			PlayerRef->AudioComp->Play();
		}
			
	}
	
}
void APulseController::ZoomIn()
{

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
	}
	else if (PlayerRef)
	{
	if (PlayerRef->bInZoom)
	{
		if (PlayerRef->bInFPS)
		{
			PlayerRef->FPSCamera->Activate();
			PlayerRef->ZoomInCamera->Deactivate();

		}
		else
		{
			PlayerRef->TPSCamera->Activate();
			PlayerRef->ZoomInTPSCamera->Deactivate();
		}
		PlayerRef->bInZoom = false;
		PlayerRef->ZoomOutHUD();
	}
	else
	{
		if (PlayerRef->bInFPS)
		{
			PlayerRef->FPSCamera->Deactivate();
			PlayerRef->ZoomInCamera->Activate();

		}
		else
		{
			PlayerRef->TPSCamera->Deactivate();
			PlayerRef->ZoomInTPSCamera->Activate();
		}
		PlayerRef->bInZoom = true;
		PlayerRef->ZoomInHUD();
	}
	}
}
void APulseController::ChangeView()
{

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		
	}
	else if (PlayerRef)
	{
	if (PlayerRef->bInFPS)
	{
		if (PlayerRef->bInZoom)
		{
			ZoomIn();
		}
		PlayerRef->FPSCamera->Deactivate();
		PlayerRef->TPSCamera->Activate();
		PlayerRef->bInFPS = false;
		PlayerRef->TPSMesh->SetOwnerNoSee(false);
		PlayerRef->FPSMesh->SetVisibility(false);
		PlayerRef->ChangeWeaponAttachment(PlayerRef->CurrentWeapon);
	}
	else
	{
		if (PlayerRef->bInZoom)
		{
			ZoomIn();
		}
		PlayerRef->FPSCamera->Activate();
		PlayerRef->TPSCamera->Deactivate();
		PlayerRef->bInFPS = true;
		PlayerRef->TPSMesh->SetOwnerNoSee(true);
		PlayerRef->FPSMesh->SetVisibility(true);
		PlayerRef->ChangeWeaponAttachment(PlayerRef->CurrentWeapon);
		
	}
	}

}
void APulseController::Throw()
{


	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());

	}
	else if (PlayerRef)
	{
		PlayerRef->ThrowGrenade();
	}
}
void APulseController::CloseCombatAttack_Implementation()
{

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		
	}
	else if (PlayerRef)
	{
		PlayerRef->CloseCombatAttack();
	}
}
bool APulseController::CloseCombatAttack_Validate()
{
	return true;
}
void APulseController::HandbrakeReleased()
{

}
void APulseController::QuitUI()
{
	if (!PlayerRef && !TurretRef)
	{
		PlayerRef = Cast<APlayerCharacter>(GetPawn());
		TurretRef = Cast<ATurret>(GetPawn());
	}
	else if (PlayerRef)
	{
		if (PlayerRef->bIsInQuitUI == false)
			PlayerRef->EnterQuitUI();
		else
			PlayerRef->ExitQuitUI();
	}
	else if (TurretRef)
	{
		if (TurretRef->bIsInQuitUI == false)
			TurretRef->EnterQuitUI();
		else
			TurretRef->ExitQuitUI();
	}
}
