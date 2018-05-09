// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleBase.h"
#include "VehicleController.h"
#include "Pulse.h"
#include "PlayerCharacter.h"
#include "GameModes/TrackGS.h"
#include "FrontVehicleWheelBase.h"
#include "RearVehicleWheelBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"
#include "PulseController.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/TrackPS.h"

#if HMD_MODULE_INCLUDED
#include "IHeadMountedDisplay.h"
#endif // HMD_MODULE_INCLUDED

const FName AVehicleBase::LookUpBinding("LookYaw");
const FName AVehicleBase::LookRightBinding("LookPitch");
const FName AVehicleBase::EngineAudioRPM("RPM");

#define LOCTEXT_NAMESPACE "VehiclePawn"

AVehicleBase::AVehicleBase()
{

	PrimaryActorTick.bCanEverTick = true;
	Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	GetMesh()->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;
	bReplicates = true;
	
	/*
	check(Vehicle4W->WheelSetups.Num() == 4);
	// Wheels/Tyres
	// Setup the wheels
	Vehicle4W->WheelSetups[0].WheelClass = UFrontVehicleWheelBase::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("PhysWheel_FL");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UFrontVehicleWheelBase::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("PhysWheel_FR");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = URearVehicleWheelBase::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("PhysWheel_BL");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = URearVehicleWheelBase::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("PhysWheel_BR");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	// Adjust the tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;

	// Engine 
	// Torque setup
	Vehicle4W->MaxEngineRPM = 5700.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);

	// Adjust the steering 
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

	// Transmission	
	// We want 4wd
	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;

	// Drive the front wheels a little more than the rear
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65;

	// Automatic gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;
	
	// Physics settings
	// Adjust the center of mass - the buggy is quite low
	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
	if (UpdatedPrimitive)
	{
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(8.0f, 0.0f, 0.0f);
	}
	
	// Set the inertia scale. This controls how the mass of the vehicle is distributed.
	Vehicle4W->InertiaTensorScale = FVector(1.0f, 1.333f, 1.2f);
	*/
	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 125.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// Create the chase camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(-125.0, 0.0f, 0.0f));
	Camera->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	InternalCameraOrigin = FVector(-34.0f, -10.0f, 50.0f);
	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());

	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	// In car HUD
	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	InCarSpeed->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	InCarSpeed->SetRelativeLocation(FVector(35.0f, -6.0f, 20.0f));
	InCarSpeed->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	InCarSpeed->SetupAttachment(GetMesh());

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	InCarGear->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	InCarGear->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
	InCarGear->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	InCarGear->SetupAttachment(GetMesh());

	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudioComponent"));
	EngineSoundComponent->bAutoActivate = false;
	EngineSoundComponent->SetupAttachment(RootComponent);

	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bIsLowFriction = false;
	bInReverseGear = false;
	NitroAmount = MaxNitroAmount;
	bIsInNitro = false;
	NonTurboEngineData = Vehicle4W->EngineSetup;
	NitroAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("NitroAudioComponent"));
	NitroAudioComponent->bAutoActivate = false;
	NitroAudioComponent->SetupAttachment(RootComponent);
	BreakAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BreakAudioComponent"));
	BreakAudioComponent->bAutoActivate = false;
	BreakAudioComponent->SetupAttachment(RootComponent);
	HandbrakeAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HandbrakeAudioComponent"));
	HandbrakeAudioComponent->bAutoActivate = false;
	HandbrakeAudioComponent->SetupAttachment(RootComponent);
}
void AVehicleBase::BreakOn()
{
	if (!bInReverseGear)
	{
		bIsBreaking = true;
		BreakAudioComponent->Play();
	}
	

	
}
void AVehicleBase::BreakOff()
{
	if (!bInReverseGear)
	{
		bIsBreaking = false;
		BreakAudioComponent->Stop();
	}
		
}
void AVehicleBase::Tick(float Delta)
{
	Super::Tick(Delta);

	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;

	// Update phsyics material
	UpdatePhysicsMaterial();

	// Update the strings used in the hud (incar and onscreen)
	UpdateHUDStrings();

	SetupInCarHUD();

	bool bHMDActive = false;
#if HMD_MODULE_INCLUDED
	if ((GEngine->HMDDevice.IsValid() == true) && ((GEngine->HMDDevice->IsHeadTrackingAllowed() == true) || (GEngine->IsStereoscopic3D() == true)))
	{
		bHMDActive = true;
	}
#endif // HMD_MODULE_INCLUDED


	// Pass the engine RPM to the sound component
	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioRPM, GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);
	KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	Gear = GetVehicleMovement()->GetCurrentGear();

	if (bIsInNitro)
	{
		NitroAmount += -Delta * 0.20f*MaxNitroAmount;
		if (NitroAmount <= 0)
		{
			bIsInNitro = false;
		}

		FVector Velocity=Vehicle4W->UpdatedPrimitive->GetPhysicsLinearVelocity()*NitroSpeed;
	
	
		if (GetVehicleMovement()->MaxEngineRPM*0.10472f*12.0f >= Velocity.Size())
		{
			Vehicle4W->UpdatedPrimitive->SetPhysicsLinearVelocity(Velocity);
		}
		else
		{
			float VZ = Vehicle4W->UpdatedPrimitive->GetPhysicsLinearVelocity().Size();
			Velocity.GetClampedToSize(-(GetVehicleMovement()->MaxEngineRPM*0.10472f*12.0f - VZ), GetVehicleMovement()->MaxEngineRPM*0.10472f*12.0f - VZ);
			//Vehicle4W->UpdatedPrimitive->SetPhysicsLinearVelocity(Velocity);
		}
			
		


		
	}
	else
	{
		if (NitroAmount <= MaxNitroAmount)
			NitroAmount += Delta * 0.05f*MaxNitroAmount;
	}
	if (bInReverseGear)
	{
		ReverseBreakMaterialRef->SetScalarParameterValue(FName("Glow Intensity"), InUseLightGlow);
		bIsBreaking = false;
	}
	else
	{
		ReverseBreakMaterialRef->SetScalarParameterValue(FName("Glow Intensity"), BaseLightGlow);
	}
	if (bIsBreaking)
	{
		BreakMaterialRef->SetScalarParameterValue(FName("Glow Intensity"), InUseLightGlow);
	}
	else
	{
		BreakMaterialRef->SetScalarParameterValue(FName("Glow Intensity"), BaseLightGlow);
	}
	if (bIsRacing)
	{
		if (!bTimerSetup)
		{
			GetWorld()->GetTimerManager().SetTimer(RaceTimeTimerHandle, this, &AVehicleBase::IncreaseRaceTime, 1.0f, true);
			bTimerSetup=true;
		}
		
	}
}

void AVehicleBase::SetFinishLocation(FVector Location)
{
	FinishLocation = Location;
}

void AVehicleBase::GameEnded()
{
	if (EndGameHUDClass)
	{
		EndGameHUD = CreateWidget<UUserWidget>(GetWorld(), EndGameHUDClass);
		EndGameHUD->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("NitroOn"));
		AVehicleController* PlayerController = Cast<AVehicleController>(GetController());
		if (PlayerController&&PlayerController->IsLocalPlayerController())
		{
			EndGameHUD->SetUserFocus(PlayerController);
			PlayerController->bShowMouseCursor = true;
		}
		PlayerHUD->RemoveFromParent();
		StopRaceCounter();
		AVehicleController* CastedController = Cast<AVehicleController>(GetController());
		if (CastedController)
		{
			CastedController->SetMoveable(false);
		}
	}
}

void AVehicleBase::StartRaceCounter()
{
	bIsRacing = true;
}

void AVehicleBase::StopRaceCounter()
{
	bIsRacing = false;
	ATrackPS* TrackPS = Cast<ATrackPS>(PlayerState);
	if (TrackPS)
	{
		TrackPS->SetRaceTime(RaceTime);
	}

}

void AVehicleBase::BeginPlay()
{
	Super::BeginPlay();
	NitroAmount = MaxNitroAmount;
	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		PlayerHUD->AddToViewport();
	}
	ATrackGS* TrackGS = Cast<ATrackGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (TrackGS)
	{
		FinishLocation = TrackGS->GetEndLocation();
		//SetActorLocation(TrackGS->GetStartLocation() + FVector(0.0f, 0.0f, 50.0f),false,nullptr,ETeleportType::TeleportPhysics);
		//UE_LOG(LogTemp, Warning, TEXT("SpawningActor At Location %s"), *TrackGS->GetStartLocation().ToString());
	}

	bool bWantInCar = false;
	bInCarCameraActive = false;
	InCarSpeed->SetVisibility(bInCarCameraActive);
	InCarGear->SetVisibility(bInCarCameraActive);
	// First disable both speed/gear displays 
	// Enable in car view if HMD is attached
#if HMD_MODULE_INCLUDED
	bWantInCar = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
#endif // HMD_MODULE_INCLUDED

	EnableIncarView(bWantInCar);
	EngineSoundComponent->SetSound(EngineSound);
	// Start an engine sound playing
	EngineSoundComponent->Play();
	if(ReverseLightsMaterial)
	ReverseBreakMaterialRef=GetMesh()->CreateDynamicMaterialInstance(GetMesh()->GetMaterialIndex(FName("ReverseLights")), ReverseLightsMaterial);
	if (BreakLightsMaterial)
	BreakMaterialRef = GetMesh()->CreateDynamicMaterialInstance(GetMesh()->GetMaterialIndex(FName("BreakLights")), BreakLightsMaterial);
	if (NitroSound)
		NitroAudioComponent->SetSound(NitroSound);
	if (BreakSound)
		BreakAudioComponent->SetSound(BreakSound);
	if (HandbreakSound)
		HandbrakeAudioComponent->SetSound(HandbreakSound);

	RaceTimeTimerDelegate.BindUFunction(this, FName("IncreaseRaceTime"));
}



void AVehicleBase::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);
	int32 Gear = GetVehicleMovement()->GetCurrentGear();

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));


	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}

}
void AVehicleBase::IncreaseRaceTime()
{
	if(bIsRacing)
	RaceTime += 1.0f;
	else
	GetWorld()->GetTimerManager().ClearTimer(RaceTimeTimerHandle);
	
}
void AVehicleBase::UpdatePhysicsMaterial()
{
	if (GetActorUpVector().Z < 0)
	{
		if (bIsLowFriction == true)
		{
			GetMesh()->SetPhysMaterialOverride(NonSlipperyMaterial);
			bIsLowFriction = false;
		}
		else
		{
			GetMesh()->SetPhysMaterialOverride(SlipperyMaterial);
			bIsLowFriction = true;
		}
	}
}
void AVehicleBase::OnInteract_Implementation(AActor* Caller)
{
	
}
void AVehicleBase::OnBeginFocus()
{
	GetMesh()->SetRenderCustomDepth(true);
}
void AVehicleBase::OnEndFocus()
{
	GetMesh()->SetRenderCustomDepth(false);
}
void AVehicleBase::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}
void AVehicleBase::SetupInCarHUD()
{
	AVehicleController* PlayerController = Cast<AVehicleController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr))
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString);
		InCarGear->SetText(GearDisplayString);

		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}
void AVehicleBase::PlayHandbrakeSound()
{
	HandbrakeAudioComponent->Play();
}
void AVehicleBase::StopHandbrakeSound()
{
	HandbrakeAudioComponent->Stop();
}
void AVehicleBase::SetNitroOn_Implementation()
{
	if (NitroAmount > 0.0f)
	{
		bIsInNitro = true;
		FVector Location1 = GetMesh()->GetSocketLocation(ExhaustSocketName1);
		FRotator Rotation1 = GetMesh()->GetSocketRotation(ExhaustSocketName1);
		NitroEffectRef1=UGameplayStatics::SpawnEmitterAttached(NitroEffect, GetMesh(), ExhaustSocketName1, Location1, Rotation1, EAttachLocation::KeepWorldPosition, true);
		FVector Location2 = GetMesh()->GetSocketLocation(ExhaustSocketName2);
		FRotator Rotation2 = GetMesh()->GetSocketRotation(ExhaustSocketName2);
		NitroEffectRef2 = UGameplayStatics::SpawnEmitterAttached(NitroEffect, GetMesh(), ExhaustSocketName2, Location2, Rotation2, EAttachLocation::KeepWorldPosition, true);
		UE_LOG(LogTemp, Warning, TEXT("Nitrosss %s"),*Location1.ToString());
		NitroAudioComponent->Play();
	}
}
bool AVehicleBase::SetNitroOn_Validate()
{
	return true;
}
void AVehicleBase::SetNitroOff_Implementation()
{
	bIsInNitro = false;
	if (NitroEffectRef1&&NitroEffectRef2)
	{
		NitroEffectRef1->DestroyComponent();
		NitroEffectRef2->DestroyComponent();
		NitroAudioComponent->Stop();
	}
	
}
bool AVehicleBase::SetNitroOff_Validate()
{
	return true;
}
void AVehicleBase::EnableIncarView(const bool bState)
{
	if (bState != bInCarCameraActive)
	{
		bInCarCameraActive = bState;

		if (bState == true)
		{
			Camera->Deactivate();
			InternalCamera->Activate();
		}
		else
		{
			InternalCamera->Deactivate();
			Camera->Activate();
		}

		InCarSpeed->SetVisibility(bInCarCameraActive);
		InCarGear->SetVisibility(bInCarCameraActive);
	}
}
void AVehicleBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVehicleBase, NitroEffect);
	DOREPLIFETIME(AVehicleBase, ExhaustSocketName1);
	DOREPLIFETIME(AVehicleBase, ExhaustSocketName2);
	//DOREPLIFETIME(AVehicleBase, NitroAmount);
	//DOREPLIFETIME(AVehicleBase, MaxNitroAmount);
	DOREPLIFETIME(AVehicleBase, NitroEffectRef1);
	DOREPLIFETIME(AVehicleBase, NitroEffectRef2);
}


#undef LOCTEXT_NAMESPACE



