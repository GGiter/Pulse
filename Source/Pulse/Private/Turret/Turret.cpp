// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "Pulse.h"
#include "Player/PlayerCharacter.h"
#include "Player/PulseController.h"
#include "Actors/Bullet.h"
#include "UnrealNetwork.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ATurret::ATurret()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	LeftGunMesh =CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftGunMesh"));
	RightGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightGunMesh"));
	MainBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBodyMesh"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	LeftGunMesh->CustomDepthStencilValue = STENCIL_FRIENDLY_OUTLINE;
	RightGunMesh->CustomDepthStencilValue = STENCIL_FRIENDLY_OUTLINE;
	MainBodyMesh->CustomDepthStencilValue = STENCIL_FRIENDLY_OUTLINE;
	BaseMesh->CustomDepthStencilValue = STENCIL_FRIENDLY_OUTLINE;

	RootComponent = BaseMesh;
	MainBodyMesh->SetupAttachment(RootComponent);
	LeftGunMesh->AttachToComponent(MainBodyMesh,FAttachmentTransformRules::KeepRelativeTransform);
	RightGunMesh->AttachToComponent(MainBodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	TraceParams = FCollisionQueryParams(FName(TEXT("HitscanTrace")), true, this);
	StartingAmmo = MaxAmmo;
	CurrentAmmo = StartingAmmo;
	Spread = MinSpread;
	Charge = 0;

	PlayerNameRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerNameRender"));
	PlayerNameRender->SetupAttachment(RootComponent);

	RightMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("RightMuzzle"));
	LeftMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("LeftMuzzle"));
	RightMuzzle->SetupAttachment(RightGunMesh);
	LeftMuzzle->SetupAttachment(LeftGunMesh);
	CurrentMuzzle = LeftMuzzle;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->bAutoActivate = false;
	AudioComp->SetupAttachment(RootComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(MainBodyMesh);

	TPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamera"));
	TPSCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepWorldTransform);
	TPSCamera->bAutoActivate = true;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
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
	
}
void ATurret::MoveGuns(float Value)
{
	if (LeftGunMesh->GetComponentRotation().Roll >= -20.0f&&LeftGunMesh->GetComponentRotation().Roll <= 20.0f)
	{
		LeftGunMesh->AddRelativeRotation(FRotator(0.0f, 0.0f, Value*MainBodyRotationSpeed));
	}
	else
	{
		if (LeftGunMesh->GetComponentRotation().Roll <= -20.0f&&Value > 0.0f)
			LeftGunMesh->AddRelativeRotation(FRotator(0.0f, 0.0f, Value*MainBodyRotationSpeed));
		if (LeftGunMesh->GetComponentRotation().Roll >= 20.0f&&Value < 0.0f)
			LeftGunMesh->AddRelativeRotation(FRotator(0.0f, 0.0f, Value*MainBodyRotationSpeed));
	}

	if (RightGunMesh->GetComponentRotation().Roll >= -20.0f&&RightGunMesh->GetComponentRotation().Roll <= 20.0f)
	{
		RightGunMesh->AddRelativeRotation(FRotator(0.0f, 0.0f, Value*MainBodyRotationSpeed));
	}
	else
	{
		if (RightGunMesh->GetComponentRotation().Roll <= -20.0f&&Value > 0.0f)
			RightGunMesh->AddRelativeRotation(FRotator(0.0f, 0.0f, Value*MainBodyRotationSpeed));
		if (RightGunMesh->GetComponentRotation().Roll >= 20.0f&&Value < 0.0f)
			RightGunMesh->AddRelativeRotation(FRotator(0.0f, 0.0f, Value*MainBodyRotationSpeed));
	}
	
}
void ATurret::MoveMainBody(float Value)
{
	MainBodyMesh->AddRelativeRotation(FRotator(0.0f, Value*MainBodyRotationSpeed,0.0f ));
}
void ATurret::StartFire()
{
	if (CurrentAmmo > 0) {

		bIsFiring = true;
		DoFire();
		float TimerDelay = FireRate > 0 ? 1 / (FireRate*0.01667) : FApp::GetDeltaTime();

		if (!FireRateHandle.IsValid())
			GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this, &ATurret::StartFire, TimerDelay, true);

	}
	else {
		StopFire();
		UE_LOG(LogTemp, Warning, TEXT("ClipSize Error"));
	}
}
void ATurret::StopFire()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireRateHandle);
	FireRateHandle.Invalidate();
}
void ATurret::DoFire()
{
	FHitResult Hit(ForceInit);
	FVector Start = CurrentMuzzle->GetComponentLocation();
	FVector End = Start + (AimDir()*MaxRange);


	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Weapon, TraceParams);

	UE_LOG(LogTemp, Warning, TEXT("Fire!"));

	if (Spread < MaxSpread)
		Spread+=SpreadIncreaseSpeed;
	CurrentAmmo--;

	SpawnFireEffect();

	SpawnBullet();
	
	if (CurrentMuzzle == LeftMuzzle)
		CurrentMuzzle = RightMuzzle;
	else
		CurrentMuzzle = LeftMuzzle;

	SimRecoil();
	
}
void ATurret::DealDamage(const FHitResult& Hit) {
	if (Hit.GetActor()) {
		float DealtDamage = BaseDamage;
		FVector ShotDir = GetActorLocation() - Hit.ImpactPoint;

		FPointDamageEvent DamageEvent;
		DamageEvent.Damage = DealtDamage;
		DamageEvent.HitInfo = Hit;
		DamageEvent.ShotDirection = ShotDir;
		DamageEvent.ShotDirection.Normalize();
	
		Hit.GetActor()->TakeDamage(DealtDamage, DamageEvent, GetController(), this);
		
		Spot_On();
	}
}
FVector ATurret::CalcSpread() {
		FVector Direction = GetActorRotation().Vector();
		float Angle = atan(Spread / 10000);
		return FMath::VRandCone(Direction, Angle);	
}
void ATurret::SpawnFireEffect() {
	FVector Location = CurrentMuzzle->GetComponentLocation();
	FRotator Rotation = CurrentMuzzle->GetComponentRotation();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShotSound, Location, Rotation, 1, 1, 0);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotEffect, FTransform(Rotation, Location, FVector(1.0f, 1.0f, 1.0f)));
}
void ATurret::SpawnImpactEffect(const FHitResult& Hit) {
	FVector Location = Hit.ImpactPoint;
	FRotator Rotation = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Location, Rotation, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location, Rotation, 1, 1, 0);
}
void ATurret::SpawnBullet()
{

	if (BulletClass)
	{
		FActorSpawnParameters SpawnParams;

		FTransform SpawnTM(CurrentMuzzle->GetComponentLocation());
	
		ABullet* Bullet = Cast<ABullet>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BulletClass, SpawnTM));
		if (Bullet)
		{
			Bullet->Instigator = Player;
			Bullet->SetOwner(this);

			
			Bullet->InitVelocity(AimDir()*MaxRange);

			UGameplayStatics::FinishSpawningActor(Bullet, SpawnTM);
		}
	}


}
FVector ATurret::AimDir()
{
		APlayerController* PC = Cast<APlayerController>(GetController());
		FVector FinalAim = FVector::ZeroVector;

		if (PC)
		{
			FVector CamLoc;
			FRotator CamRot;
			PC->GetPlayerViewPoint(CamLoc, CamRot);

			FinalAim = CamRot.Vector();
		}
		return FinalAim;

}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsFiring)
	{
		if (Spread - SpreadDecreaseSpeed >= MinSpread)
			Spread -= SpreadDecreaseSpeed;
	}
	if (!bIsFiring && CurrentAmmo<StartingAmmo) {
		Charge += RechargeAmount * DeltaTime;
		if (Charge > 1) {
			CurrentAmmo += FMath::Fmod(Charge, StartingAmmo - CurrentAmmo);
			Charge -= 1;
		}
	}
	if (bLeftGunRecoil)
	{
		RecoilCheckLeft += LeftGunMesh->GetForwardVector().Size();
		LeftGunMesh->AddRelativeLocation(-1.0f*LeftGunMesh->GetForwardVector());
		if (RecoilCheckLeft>20.0f)
		{
			bLeftGunRecoil = false;
			RecoilCheckLeft = 0.0f;
		}
	}
	if (bRightGunRecoil)
	{
		RecoilCheckRight += RightGunMesh->GetForwardVector().Size();
		RightGunMesh->AddRelativeLocation(-1.0f*RightGunMesh->GetForwardVector());
		if (RecoilCheckRight>20.0f)
		{
			bRightGunRecoil = false;
			RecoilCheckRight = 0.0f;
		}
	}
}

// Called to bind functionality to input
void ATurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void ATurret::OnInteract_Implementation(AActor* Caller)
{
	APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(Caller);
	if (PlayerRef)
	{
		PlayerRef->SetPlayerTurret(this);
		Player = PlayerRef;
		PlayerRef->DeactiveWidgets();
		APulseController* PlayerController = Cast<APulseController>(PlayerRef->GetController());
		if (PlayerController)
		{

			PlayerController->UnPossess();
			PlayerController->Possess(this);
			PlayerController->PlayerRef = NULL;
			PlayerRef->SetActorHiddenInGame(true);
			PlayerRef->SetActorEnableCollision(false);
			ActivateWidgets();

		}
	}
}
void ATurret::OnBeginFocus()
{
	LeftGunMesh->SetRenderCustomDepth(true);
	RightGunMesh->SetRenderCustomDepth(true);
	MainBodyMesh->SetRenderCustomDepth(true);
	BaseMesh->SetRenderCustomDepth(true);

}
void ATurret::OnEndFocus()
{
	LeftGunMesh->SetRenderCustomDepth(false);
	RightGunMesh->SetRenderCustomDepth(false);
	MainBodyMesh->SetRenderCustomDepth(false);
	BaseMesh->SetRenderCustomDepth(false);
}
float ATurret::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	
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
	CurrentHealth -= ActualDamage;
	if (CurrentHealth <= 0) {
		OnDeath();
	}
	AudioComp->SetSound(DamageTakenCue);
	AudioComp->Play();
	return ActualDamage;
}
void ATurret::OnDeath()
{
	DeactivateWidgets();
	FVector PlayerLocation{ this->GetActorLocation().X + 20,this->GetActorLocation().Y + 20,this->GetActorLocation().Z + 20 };
	Player->SetActorLocation(PlayerLocation);
	Player->SetActorHiddenInGame(false);
	Player->SetActorEnableCollision(true);
	APulseController* PlayerController = Cast<APulseController>(GetController());
	if (PlayerController)
	{

		PlayerController->UnPossess();
		PlayerController->Possess(Player);
		Player = nullptr;
		Destroy();

	}

	
}
void ATurret::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATurret, CurrentHealth);
	DOREPLIFETIME(ATurret, MaxHealth);

}
void ATurret::UpdateHUD()
{
	BP_UpdateHUD();
}
void ATurret::AddBossUI(UUserWidget * NewBossUI)
{
	if (BossUI)
	{
		BossUI->RemoveFromParent();
	}
	BossUI = NewBossUI;
	BossUI->AddToViewport();
}
void ATurret::EnterQuitUI()
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
void ATurret::ExitQuitUI()
{
	bIsInQuitUI = false;
	if (ExitUI)
		ExitUI->RemoveFromParent();
	WaveHUD->AddToViewport();
	PlayerHUD->AddToViewport();
	if (BossUI)
		BossUI->AddToViewport();
}
void ATurret::SimRecoil()
{
	if (CurrentMuzzle == LeftMuzzle)
	{
		bLeftGunRecoil = true;
		bRightGunRecoil = false;
	}
	else
	{
		bLeftGunRecoil = false;
		bRightGunRecoil = true;
	}
}
void ATurret::ActivateWidgets()
{
	if (PlayerHUD)
		PlayerHUD->AddToViewport();
	if (WaveHUD)
		WaveHUD->AddToViewport();
}
void ATurret::DeactivateWidgets()
{
	if (PlayerHUD)
		PlayerHUD->RemoveFromParent();
	if (WaveHUD)
		WaveHUD->RemoveFromParent();
}
