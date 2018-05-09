// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickup.h"
#include "Pulse.h"

#include "PlayerCharacter.h"

void AAmmoPickup::OnInteract_Implementation(AActor* Caller)
{
	Super::OnInteract(Caller);
	APlayerCharacter * Player = Cast<APlayerCharacter>(Caller);
	if (Player)
	{
		
		if (bIsActive)
			Player->AddAmmo(AmmoType, Amount);
		bIsActive = false;
		OnPickedUp();
		if (PickupSound)
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



