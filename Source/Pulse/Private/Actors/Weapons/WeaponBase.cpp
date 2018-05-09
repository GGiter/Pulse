// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Pulse.h"
#include "AI/Characters/FireEnemyCharacterBase.h"
#include "PlayerCharacter.h"
#include "Actors/Bullet.h"
#include "PulseController.h"

AWeaponBase::AWeaponBase()
{

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepWorldTransform);

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	TraceParams = FCollisionQueryParams(FName(TEXT("HitscanTrace")), true, this);
	Spread = MinSpread;
	if (FMath::Fmod(CurrentAmmo, MaxClipSize) == 0)
		ClipSize = MaxClipSize;
	else
		ClipSize = FMath::Fmod(CurrentAmmo, MaxClipSize);


}
void AWeaponBase::Tick(float DeltaTime)
{
	if (!bIsFiring)
	{
		if (Spread - SpreadDecreaseSpeed >= MinSpread)
			Spread -= SpreadDecreaseSpeed;
	}

}

void AWeaponBase::BeginPlay() {
	Super::BeginPlay();



	if (bSpawnFull) CurrentAmmo = MaxAmmo;
	else CurrentAmmo = 0;

	Reload();
}
void AWeaponBase::AddAmmo(float Amount) {
	CurrentAmmo = FMath::Clamp(CurrentAmmo + Amount, CurrentAmmo, MaxAmmo);
}

void AWeaponBase::SetAmmo(float Amount) {
	CurrentAmmo = Amount;
}
void AWeaponBase::DealDamage(const FHitResult& Hit) {
	if (Hit.GetActor()) {
		float DealtDamage = BaseDamage;
		FVector ShotDir = GetActorLocation() - Hit.ImpactPoint;

		FPointDamageEvent DamageEvent;
		DamageEvent.Damage = DealtDamage;
		DamageEvent.HitInfo = Hit;
		DamageEvent.ShotDirection = ShotDir;
		DamageEvent.ShotDirection.Normalize();
		ACharacter* Char = Cast<ACharacter>(GetOwner());
		if (Char)
		{
			Hit.GetActor()->TakeDamage(DealtDamage, DamageEvent, Char->GetController(), this);
		}
		if(OwningPlayer)
		OwningPlayer->Spot_On();
	}
}
void AWeaponBase::StartFire() {
	if (ClipSize > 0) {

		bIsFiring = true;
		DoFire();
		float TimerDelay = FireRate > 0 ? 1 / (FireRate*0.01667) : FApp::GetDeltaTime();

		if (!FireRateHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this, &AWeaponBase::StartFire, TimerDelay, true);
		
	}
	else {
		StopFire();
		UE_LOG(LogTemp, Warning, TEXT("ClipSize Error"));
	}
}
void AWeaponBase::StopFire() {

	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireRateHandle);
	FireRateHandle.Invalidate();
	
}
void AWeaponBase::DoFire() {
	if(GetOwner())
	{
		FHitResult Hit(ForceInit);
		FVector Start = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		FVector End = Start + (AimDir()*MaxRange);

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Weapon, TraceParams);

		UE_LOG(LogTemp, Warning, TEXT("Fire!"));

		if (Spread < MaxSpread)
			IncreaseSpread(SpreadIncreaseSpeed);
		ClipSize--;

		SpawnFireEffect();
		
		if (bUsesProjectile)
		{
			SpawnBullet();
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), true, 1, 0, 0.7f);
			if (Hit.GetActor()) {
				UE_LOG(LogTemp, Warning, TEXT("Hit! %s"), *Hit.GetActor()->GetName());
				DealDamage(Hit);
				SpawnImpactEffect(Hit);
			}
		}
	}
}
FVector AWeaponBase::CalcSpread() {
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (Char) {
		FVector Direction = Char->GetActorRotation().Vector();
		float Angle = atan(Spread / 10000);
		return FMath::VRandCone(Direction, Angle);
	}
	else {
		FVector Direction = GetActorRotation().Vector();
		float Angle = atan(Spread / 10000);
		return FMath::VRandCone(Direction, Angle);
	}
}
void AWeaponBase::SpawnFireEffect() {
	FVector Location = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	FRotator Rotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	UGameplayStatics::SpawnEmitterAttached(ShotEffect, WeaponMesh, MuzzleSocketName, Location, Rotation, EAttachLocation::KeepWorldPosition, true);
	BeamEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotEffect, Location, Rotation, false);
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (BeamEffect&&Char)
	{
		BeamEffect->SetVisibility(true);
		BeamEffect->SetBeamSourcePoint(0, Location + Char->GetControlRotation().Vector() * 500, 0);
		//GetWorld()->GetTimerManager().SetTimer(BeamEffectTimerHandle, this, &AWeaponBase::BeamEffectVisibility, MaxBeamEffectTime, false);


	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),ShotSound,Location,Rotation,1,1,0);
}
void AWeaponBase::SpawnImpactEffect(const FHitResult& Hit) {
	FVector Location = Hit.ImpactPoint;
	FRotator Rotation = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Location, Rotation, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location, Rotation, 1, 1, 0);
	if (BeamEffect)
	{

		BeamEffect->SetBeamTargetPoint(0, Location, 0);
		//GetWorld()->GetTimerManager().SetTimer(BeamEffectTimerHandle, this, &AWeaponBase::BeamEffectVisibility, MaxBeamEffectTime, false);
		BeamEffect->SetVisibility(false);
	}
}
void AWeaponBase::OnInteract_Implementation(AActor* Caller) {
	APlayerCharacter* Player = Cast<APlayerCharacter>(Caller);
	if (Player) {
		
			MeshComp->SetVisibility(false);
			MeshComp->SetSimulatePhysics(false);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UE_LOG(LogTemp, Warning, TEXT("Visiblity"));
			Player->AddToInventory(this, Player->SetWeaponSlot(this));
			Player->Item_PopUp(WeaponName);
		
		
	}
	else
	{
		MeshComp->SetVisibility(false);
		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
void AWeaponBase::Reload()
{
	if (bCanReload)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloading"));
		if (CurrentAmmo > 0)
		{
			if (CurrentAmmo - (MaxClipSize - ClipSize) >= 0)
			{
				CurrentAmmo -= (MaxClipSize - ClipSize);
				ClipSize += (MaxClipSize - ClipSize);
			}
			else
			{
				ClipSize += CurrentAmmo;
				CurrentAmmo = 0;	
			}
			
			if(ReloadSeq)
			WeaponMesh->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage(ReloadSeq, TEXT("Normal"), 0.1f, 0.1f, 1.0f);
			
		}
	}
	
}
void AWeaponBase::IncreaseSpread(float Value)
{
	Spread += Value;
}
void AWeaponBase::AddMagazine()
{
	CurrentAmmo += ClipSize;
}
USkeletalMeshComponent * AWeaponBase::GetMesh() const
{
	return WeaponMesh;
}
void AWeaponBase::ChangeOwner(APlayerCharacter* Player)
{
	OwningPlayer = Player;
}
void AWeaponBase::SpawnBullet()
{
	
	if (BulletClass)
	{
		FActorSpawnParameters SpawnParams;
		
		FTransform SpawnTM(WeaponMesh->GetSocketLocation(MuzzleSocketName));
		
		ABullet* Bullet=Cast<ABullet>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BulletClass, SpawnTM));
		if (Bullet)
		{
			ACharacter* Char = Cast<ACharacter>(GetOwner());
			if(Char)
			Bullet->Instigator = Char;
			Bullet->SetOwner(this);

			// Here, in the InitVelocity, I pass the ShootDir variable
			Bullet->InitVelocity(AimDir()*MaxRange);

			UGameplayStatics::FinishSpawningActor(Bullet, SpawnTM);
		}
	}
	
		
}
FVector AWeaponBase::AimDir()
{
	if (OwningPlayer)
	{
		APlayerController* PC = Cast<APlayerController>(OwningPlayer->GetController());
		FVector FinalAim = FVector::ZeroVector;

		if (PC)
		{
			FVector CamLoc;
			FRotator CamRot;
			PC->GetPlayerViewPoint(CamLoc, CamRot);

			FinalAim = CamRot.Vector();
		}
		else if (OwningPlayer)
		{
			FinalAim = OwningPlayer->GetBaseAimRotation().Vector();
		}
		return FinalAim;
	}
	else
	{
		if (GetOwner())
		{
			APawn* P = Cast<APawn>(GetOwner());
			return P->GetBaseAimRotation().Vector();
		}
	}
	
	return { 0,0,0 };

}
