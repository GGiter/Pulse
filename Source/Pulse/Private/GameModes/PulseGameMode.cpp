// Fill out your copyright notice in the Description page of Project Settings.

#include "PulseGameMode.h"
#include "Pulse.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h"
#include "AI/Characters/BossCharacter.h"
#include "AI/Characters/BaseEnemyCharacter.h"
#include "Turret/Turret.h"
#include "PulseGS.h"
#include "PulsePS.h"
#include "PulsePlayerStart.h"

APulseGameMode::APulseGameMode()
{

}
void APulseGameMode::BeginPlay()
{

	TArray<AActor*> Spawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Spawns);
	for (AActor* Spawn : Spawns)
	{
		if (Spawn->ActorHasTag(SpawnTag)) {
			AISpawnPoints.Add(Spawn);
		}
	}

	
}
void APulseGameMode::UpdateHUD()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), Players);
	for (AActor* Player : Players)
	{
		APlayerCharacter* CastedPlayer = Cast<APlayerCharacter>(Player);
		if (CastedPlayer && CastedPlayer->IsPlayerControlled())
		{
			CastedPlayer->UpdateHUD();
		}
	}
	Players.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurret::StaticClass(), Players);
	for (AActor* Player : Players)
	{
		ATurret* CastedPlayer = Cast<ATurret>(Player);
		if (CastedPlayer && CastedPlayer->IsPlayerControlled())
		{
			CastedPlayer->UpdateHUD();
		}
	}
}
void APulseGameMode::SetWaveDelay(float Delay)
{
	WaveDelay = Delay;
}
void APulseGameMode::SetMaxWaves(int32 Max)
{
	MaxWaves = Max;
}
void APulseGameMode::SetWaveInfo(const TArray<FWaveInfo>& newWaveInfo)
{
	WaveInfo = newWaveInfo;
}
void APulseGameMode::SetSpawnDelay(float NewSpawnDelay)
{
	SpawnDelay = NewSpawnDelay;
}
void APulseGameMode::Killed(AController* Killer, AController* Victim)
{
	ABaseEnemyCharacter* VictimPawn = Cast<ABaseEnemyCharacter>(Victim->GetPawn());
	if (VictimPawn && !VictimPawn->IsPlayerControlled()) {
		PulseGS->AddEnemiesRemaining(-1);
		UpdateHUD();
	}
	APulsePS* PulsePS = Cast<APulsePS>(Killer->PlayerState);
	if (PulsePS)
	{
		PulsePS->AddMoney(VictimPawn->GetMoneyReward());
		UpdateHUD();
	}
	if (PulseGS->GetEnemiesRemaining() <= 0)
	{
		EndWave();
	}
}
void APulseGameMode::BeginWave()
{

	GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
	WaveTimerHandle.Invalidate();

	PulseGS->SetIsWaveActive(true);
	PulseGS->SetCurrentWave(PulseGS->GetCurrentWave() + 1);
	UpdateHUD();
	BeginSpawning();

}
void APulseGameMode::EndWave()
{
	PulseGS->SetIsWaveActive(false);
	UpdateHUD();
	if (PulseGS->GetCurrentWave() >= MaxWaves) {
		EndMatch();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &APulseGameMode::BeginWave, WaveDelay, false);
	}
}
void APulseGameMode::BeginSpawning()
{
	SpawnedOfType.Empty();
	int32 CurrentWave = PulseGS->GetCurrentWave();
	for (int i = 0; i < WaveInfo[CurrentWave - 1].SpawnInfo.Num(); ++i) {
		SpawnedOfType.Add(0);
	}
	EnemyToSpawn = 0;
	EnemiesSpawned = 0;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &APulseGameMode::SpawnEnemy, SpawnDelay, true);
}
void APulseGameMode::SpawnEnemy()
{
	if (AISpawnPoints.Num() < 1 || WaveInfo.Num() < 1) {
		UE_LOG(LogTemp, Error, TEXT("Something went wrong!"));
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		SpawnTimerHandle.Invalidate();
	}
	int32 CurrentWave = PulseGS->GetCurrentWave();
	if (EnemiesSpawned < WaveInfo[CurrentWave - 1].TotalNumberOfEnemies) {
		FSpawnInfo SpawnInfo = WaveInfo[CurrentWave - 1].SpawnInfo[EnemyToSpawn];

		if (SpawnedOfType[EnemyToSpawn] < SpawnInfo.MaxAmount) {
			float Prob = FMath::RandRange(0.f, 1.f);
			if (Prob <= SpawnInfo.Probability) {
				int32 SpawnIndex = FMath::RandRange(0, AISpawnPoints.Num() - 1);
				FVector SpawnLoc = AISpawnPoints[SpawnIndex]->GetActorLocation();
				FRotator SpawnRot = AISpawnPoints[SpawnIndex]->GetActorRotation();
				
				UE_LOG(LogTemp, Warning, TEXT("SpawningActor %d"),EnemyToSpawn);
				AActor* Enemy=GetWorld()->SpawnActor<AActor>(SpawnInfo.EnemyClass, SpawnLoc, SpawnRot);

				ABossCharacter* BossRef = Cast<ABossCharacter>(Enemy);
				if (BossRef)
				{
					UUserWidget* Temp= CreateWidget<UUserWidget>(GetWorld(), BossUIClass);
					UBossWidget* BossUI = Cast<UBossWidget>(Temp);
					if (BossUI)
					{
						BossUI->SetBossRef(BossRef);
						TArray<AActor*> Players;
						UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), Players);
						for (AActor* Player : Players)
						{
							APlayerCharacter* CastedPlayer = Cast<APlayerCharacter>(Player);
							if (CastedPlayer && CastedPlayer->IsPlayerControlled())
							{
								CastedPlayer->AddBossUI(BossUI);
							}
						}
						Players.Empty();
						UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurret::StaticClass(), Players);
						for (AActor* Player : Players)
						{
							ATurret* CastedPlayer = Cast<ATurret>(Player);
							if (CastedPlayer && CastedPlayer->IsPlayerControlled())
							{
								CastedPlayer->AddBossUI(BossUI);
							}
						}
					}
				}

				EnemiesSpawned++;
				SpawnedOfType[EnemyToSpawn]++;
				PulseGS->AddEnemiesRemaining(1);
				UpdateHUD();
			}
		}
		if (EnemyToSpawn >= WaveInfo[CurrentWave - 1].SpawnInfo.Num() - 1)
		{
			EnemyToSpawn = 0;
		}
		else
		{
			EnemyToSpawn++;
		}
	}
	else {
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		SpawnTimerHandle.Invalidate();
	}
}
void APulseGameMode::StartMatch()
{
	if (!HasMatchStarted()) {
		EndWave();
	}
	Super::StartMatch();
	UpdateHUD();
}
void APulseGameMode::EndMatch()
{
	Super::EndMatch();
	UE_LOG(LogTemp, Warning, TEXT("You've won the match"));
	UpdateHUD();
}
void APulseGameMode::InitGameState()
{
	Super::InitGameState();
	PulseGS = Cast<APulseGS>(GameState);
	if (PulseGS&&!bIsInEditor)
	{
		PulseGS->LoadVariables(0);
		SetMaxWaves(PulseGS->GetMaxWaves());
		SetWaveDelay(PulseGS->GetWaveDelay());
		SetSpawnDelay(PulseGS->GetSpawnDelay());
		SetWaveInfo(PulseGS->GetWaveInfo());
	}
	else if (PulseGS)
	{
		PulseGS->LoadVariables(0);
		SetMaxWaves(PulseGS->GetMaxWaves());
		SetWaveDelay(PulseGS->GetWaveDelay());
		SetSpawnDelay(PulseGS->GetSpawnDelay());
	}
	
}
TArray<TSubclassOf<AWeaponBase>> APulseGameMode::GetStartingWeapons() const
{
	return StartingWeapons;
}
AActor* APulseGameMode::ChoosePlayerStart_Implementation(AController* Player) {
	TArray<APlayerStart*> PreferredSpawns;
	TArray<APlayerStart*> FallbackSpawns;

	APlayerStart* BestStart = NULL;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* TestSpawn = *It;
		if (TestSpawn->IsA<APlayerStartPIE>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			BestStart = TestSpawn;
			break;
		}
		else
		{
			if (IsSpawnpointAllowed(TestSpawn, Player))
			{
				if (IsSpawnpointPreferred(TestSpawn, Player))
				{
					PreferredSpawns.Add(TestSpawn);
				}
				else
				{
					FallbackSpawns.Add(TestSpawn);
				}
			}
		}
	}


	if (BestStart == NULL)
	{
		if (PreferredSpawns.Num() > 0)
		{
			BestStart = PreferredSpawns[FMath::RandHelper(PreferredSpawns.Num())];
		}
		else if (FallbackSpawns.Num() > 0)
		{
			BestStart = FallbackSpawns[FMath::RandHelper(FallbackSpawns.Num())];
		}
	}

	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}

bool APulseGameMode::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const
{
	APulsePlayerStart* PlayerSpawnPoint = Cast<APulsePlayerStart>(SpawnPoint);
	if (PlayerSpawnPoint)
	{
		
		if (PlayerSpawnPoint->bNotForPlayers )
		{
			return false;
		}
	}

	return true;
}

bool APulseGameMode::IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Player) const
{
	ACharacter* MyPawn = Cast<ACharacter>((*DefaultPawnClass)->GetDefaultObject<ACharacter>());
	

	if (MyPawn&&Player)
	{
		const FVector SpawnLocation = SpawnPoint->GetActorLocation();
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			ACharacter* OtherPawn = Cast<ACharacter>(*It);
			if (OtherPawn && OtherPawn != MyPawn)
			{
				const float CombinedHeight = (MyPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * 2.0f;
				const float CombinedRadius = MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
				const FVector OtherLocation = OtherPawn->GetActorLocation();

				// check if player start overlaps this pawn
				if (FMath::Abs(SpawnLocation.Z - OtherLocation.Z) < CombinedHeight && (SpawnLocation - OtherLocation).Size2D() < CombinedRadius)
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

void APulseGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
		UE_LOG(LogTemp, Warning, TEXT("Adding starting weapons to playerstate"));
		APulsePS* PulsePS = Cast<APulsePS>(NewPlayer->PlayerState);
		PulsePS->SetStaringWeapons(StartingWeapons);
	
}

