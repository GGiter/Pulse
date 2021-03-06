// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPickup.h"
#include "Pulse.h"
#include "PlayerCharacter.h"


void AHealthPickup::OnInteract_Implementation(AActor* Caller)
{
	Super::OnInteract(Caller);
	APlayerCharacter* Player = Cast<APlayerCharacter>(Caller);
	if (Player)
	{
		if(bIsActive)
		Player->Heal(Amount);
		bIsActive = false;
		OnPickedUp();
		if(PickupEffect)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupEffect, GetActorLocation());
		if(PickupSound)
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		if (bCanRespawn)
		{
			FTimerHandle RespawnTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &APickupBase::RespawnPickup, RespawnDelay + FMath::RandHelper(RespawnDelayRange), false);
		}
		else
		{
			Destroy();
		}
		
	}
}

