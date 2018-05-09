// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleController.h"
#include "Blueprint/UserWidget.h"

void AVehicleController::BeginPlay()
{
	Super::BeginPlay();


}

void AVehicleController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAxis("MoveX", this, &AVehicleController::MoveX);
		InputComponent->BindAxis("MoveY", this, &AVehicleController::MoveY);
		InputComponent->BindAxis("LookPitch", this, &AVehicleController::AddPitchInput);
		InputComponent->BindAxis("LookYaw", this, &AVehicleController::AddYawInput);
		InputComponent->BindAction("Sprint", IE_Pressed, this, &AVehicleController::NitroOn);
		InputComponent->BindAction("Sprint", IE_Released, this, &AVehicleController::NitroOff);
		InputComponent->BindAction("ChangeView", IE_Pressed, this, &AVehicleController::ChangeView);
		InputComponent->BindAction("Jump", IE_Pressed, this, &AVehicleController::HandbrakePressed);
		InputComponent->BindAction("Jump", IE_Released, this, &AVehicleController::HandbrakeReleased);
	}
}

void AVehicleController::MoveX(float Value)
{
	if (VehicleRef == nullptr)
	{
		VehicleRef = Cast<AVehicleBase>(GetPawn());
	}
	else if(bMoveable)
	{
		VehicleRef->GetVehicleMovementComponent()->SetThrottleInput(Value);
		if (Value < 0.0f)
			VehicleRef->BreakOn();
		else
			VehicleRef->BreakOff();
	}
	
}


void AVehicleController::MoveY(float Value)
{
	if (VehicleRef == nullptr)
	{
		VehicleRef = Cast<AVehicleBase>(GetPawn());
	}
	else if (bMoveable)
	{
		VehicleRef->GetVehicleMovementComponent()->SetSteeringInput(Value);
	}
	
}

void AVehicleController::HandbrakeReleased()
{
	if (VehicleRef == nullptr)
	{
		VehicleRef = Cast<AVehicleBase>(GetPawn());
	}
	else
	{
		VehicleRef->GetVehicleMovementComponent()->SetHandbrakeInput(false);
		VehicleRef->StopHandbrakeSound();
	}
	
}

void AVehicleController::HandbrakePressed()
{
	if (VehicleRef == nullptr)
	{
		VehicleRef = Cast<AVehicleBase>(GetPawn());
	}
	else
	{
		VehicleRef->GetVehicleMovementComponent()->SetHandbrakeInput(true);
		VehicleRef->PlayHandbrakeSound();
	}
	
}

void AVehicleController::ChangeView()
{
	if (VehicleRef == nullptr)
	{
		VehicleRef = Cast<AVehicleBase>(GetPawn());
	}
	else
	{
		VehicleRef->OnToggleCamera();
	}
}

void AVehicleController::NitroOn_Implementation()
{
	if (VehicleRef == nullptr)
	{
		VehicleRef = Cast<AVehicleBase>(GetPawn());
	}
	else
	{
		VehicleRef->SetNitroOn();
	}
}
bool AVehicleController::NitroOn_Validate()
{
	return true;
}
void AVehicleController::NitroOff_Implementation()
{
	if (VehicleRef == nullptr)
	{
		VehicleRef = Cast<AVehicleBase>(GetPawn());
	}
	else
	{
		VehicleRef->SetNitroOff();
	}
}
bool AVehicleController::NitroOff_Validate()
{
	return true;
}
void AVehicleController::SetMoveable(bool bNewMoveable)
{
	bMoveable = bNewMoveable;
}