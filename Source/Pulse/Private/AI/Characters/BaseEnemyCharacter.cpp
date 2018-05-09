// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemyCharacter.h"
#include "Pulse.h"
#include "Actors/Weapons/WeaponBase.h"
#include "PlayerCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "GameModes/PulseGameMode.h"
#include "PlayerCharacter.h"
#include "Actors/Weapons/SideArmsBase.h"
#include "AI/Controllers/BaseAIController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"


// Sets default values
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//If I forget to adjust the enemy's damage I will simply get one-shoted!
	BaseDamageAmount = 200000.f;

	MaxHealth = 100.f;
	Health = MaxHealth;
	//Initializing the pawn sensing component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	//Set the peripheral vision angle to 90 degrees
	PawnSensingComp->SetPeripheralVisionAngle(90.f);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->bAutoActivate = false;
	AudioComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UBaseEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	
}
void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ABaseEnemyCharacter::EnableHunting);
	}
}
float ABaseEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	
		Health -= DamageAmount;

		if (Health > 0)
		{
			AudioComp->SetSound(DamageTakenCue);
			AudioComp->Play();
		}
		else
		{
			OnDeath(DamageCauser);

		}

		LastAttack = GetWorld()->GetRealTimeSeconds();
	
	bIsAttacked = true;
	GetWorld()->GetTimerManager().SetTimer(HealthDelayTimerHandle, this, &ABaseEnemyCharacter::StopHealthWidget, HealthDelayTime, false);

	

	return Health;
}
void ABaseEnemyCharacter::OnDeath(AActor* Caller)
{
	
	APulseGameMode* PulseGM = GetWorld()->GetAuthGameMode<APulseGameMode>();
	if (PulseGM)
	{
		APlayerCharacter* Killer;
		AWeaponBase* Weapon = Cast<AWeaponBase>(Caller);
		ASideArmsBase* SWeapon = Cast<ASideArmsBase>(Caller);		
		if (Weapon&&GetController())
		{
			Killer = Cast<APlayerCharacter>(Weapon->GetOwner());
			if (Killer)
				PulseGM->Killed(Killer->GetController(), GetController());
			
		}
		else if (SWeapon&&GetController())
		{
			Killer = Cast<APlayerCharacter>(SWeapon->GetOwner());
			if (Killer)
				PulseGM->Killed(Killer->GetController(), GetController());
			
		}
		else
		{
			ATurret* KillerTurret = Cast<ATurret>(Caller);
			if (KillerTurret)
				PulseGM->Killed(KillerTurret->GetController(), GetController());
		}
			
		
			
	}
	bIsAttacked = false;
	DestroyWeapon();
	if(DeathMat)
	StartDeathAnim();
	if (Controller) GetController()->UnPossess();
}

void ABaseEnemyCharacter::StartDeathAnim()
{
	DeathDynMat = UMaterialInstanceDynamic::Create(DeathMat, this);
	GetMesh()->SetMaterial(0, DeathDynMat);
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &ABaseEnemyCharacter::DeathAnim, 0.01f, false);

}

void ABaseEnemyCharacter::DeathAnim()
{
	GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
	DeathTime += 0.01f;
	if (DeathTime >= EndDeathTime)
	{
		Destroy();
		SetActorEnableCollision(false);
		
	}
	else
	{
		DeathDynMat->SetScalarParameterValue(FName("Length"), FMath::GetMappedRangeValueClamped(FVector2D(0.0f, EndDeathTime), FVector2D(0.0f, 1.0f), DeathTime));
		GetMesh()->SetMaterial(0, DeathDynMat);
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &ABaseEnemyCharacter::DeathAnim, 0.01f, false);
		
	}
}


void ABaseEnemyCharacter::EnableHunting(APawn* Pawn)
{

	ABaseAIController* AICon = Cast<ABaseAIController>(GetController());
	if (AICon) {
		AICon->SetNewTarget(Pawn);
	}
}
void ABaseEnemyCharacter::SetControllerDistance(float Distance)
{

}
bool ABaseEnemyCharacter::IsRunning() const
{
	if (!GetCharacterMovement())
	{
		return false;
	}
	return (bWantsToRun || bWantsToRunToggled)&&!GetVelocity().IsZero() && (GetVelocity().GetSafeNormal2D() | GetActorForwardVector()) > -0.1;
}
FRotator ABaseEnemyCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();
	return AimRotLS;
}
void ABaseEnemyCharacter::Attack()
{
	
}
void ABaseEnemyCharacter::StopHealthWidget()
{
	bIsAttacked = true;
	GetWorld()->GetTimerManager().ClearTimer(HealthDelayTimerHandle);
}

void ABaseEnemyCharacter::DestroyWeapon()
{
}
