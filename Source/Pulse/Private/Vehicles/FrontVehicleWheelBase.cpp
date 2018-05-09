// Fill out your copyright notice in the Description page of Project Settings.

#include "FrontVehicleWheelBase.h"
#include "Pulse.h"


UFrontVehicleWheelBase::UFrontVehicleWheelBase()
{
	ShapeRadius = 18.f;
	ShapeWidth = 15.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 40.f;

	// Setup suspension forces
	SuspensionForceOffset = -4.0f;
	SuspensionMaxRaise = 8.0f;
	SuspensionMaxDrop = 12.0f;
	SuspensionNaturalFrequency = 9.0f;
	SuspensionDampingRatio = 1.05f;
}


