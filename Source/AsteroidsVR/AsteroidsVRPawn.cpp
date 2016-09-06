// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AsteroidsVR.h"
#include "AsteroidsVRPawn.h"

AAsteroidsVRPawn::AAsteroidsVRPawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("StaticMesh'/Game/Models/viperMkII.viperMkII'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	PlaneMesh->SetSimulatePhysics(true);
	PlaneMesh->SetEnableGravity(false);
	PlaneMesh->SetConstraintMode(EDOFMode::SixDOF);
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 375; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 130.f);
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	ContinualAcceleration = 2000.f;
	BrakeSpeed = 4000.0f;
	MinSpeed = 50.f;
	MaxSpeed = 4000.f;
	CurrentForwardSpeed = 50.f;

	RollSpeed = 150.0f;
	PitchSpeed = -70.0f;
	YawSpeed = 150.0f;

}

void AAsteroidsVRPawn::Tick(float DeltaSeconds)
{
	auto NewForwardSpeed = CurrentForwardSpeed;

	auto isRoll = LeftControl.Y * RightControl.Y < 0;

	if (!isRoll)
	{
		CurrentPitchSpeed = (LeftControl.Y + RightControl.Y) *0.5f;
	}
	else
	{
		CurrentRollSpeed = (LeftControl.Y + -1.0f*RightControl.Y) * 0.5f;
	}


	auto isBrake = LeftControl.X * RightControl.X < 0;

	if (!isBrake)
	{
		NewForwardSpeed += ContinualAcceleration * DeltaSeconds;
		CurrentYawSpeed = (LeftControl.X + RightControl.X) *0.5f;
	}
	else
	{
		NewForwardSpeed += (LeftControl.X + -1.0f*RightControl.X) * 0.5f * BrakeSpeed * DeltaSeconds;
	}

	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);



	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * PitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * YawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * RollSpeed* DeltaSeconds;


	CurrentRollSpeed *= 0.5f;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);

	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

	print(*FString::SanitizeFloat(CurrentYawSpeed));
	print(*FString::SanitizeFloat(CurrentPitchSpeed));
	print(*FString::SanitizeFloat(CurrentRollSpeed));
	print(TEXT(""));
}

void AAsteroidsVRPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation(RootComponent);
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}


void AAsteroidsVRPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LeftUp", this, &AAsteroidsVRPawn::LeftUp);
	PlayerInputComponent->BindAxis("LeftRight", this, &AAsteroidsVRPawn::LeftRight);
	PlayerInputComponent->BindAxis("RightUp", this, &AAsteroidsVRPawn::RightUp);
	PlayerInputComponent->BindAxis("RightRight", this, &AAsteroidsVRPawn::RightRight);
}

void AAsteroidsVRPawn::ThrustInput(float Val)
{
	//// Is there no input?
	//bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	//// If input is not held down, reduce speed
	//float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	//// Calculate new speed
	//float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	//// Clamp between MinSpeed and MaxSpeed
	//CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void AAsteroidsVRPawn::MoveUpInput(float Val)
{
	//// Target pitch speed is based in input
	//float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	//// When steering, we decrease pitch slightly
	//TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	//// Smoothly interpolate to target pitch speed
	//CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AAsteroidsVRPawn::MoveRightInput(float Val)
{
	//// Target yaw speed is based on input
	//float TargetYawSpeed = (Val * TurnSpeed);

	//// Smoothly interpolate to target yaw speed
	//CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	//// Is there any left/right input?
	//const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	//// If turning, yaw value is used to influence roll
	//// If not turning, roll to reverse current roll value
	//float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	//// Smoothly interpolate roll speed
	//CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}


void AAsteroidsVRPawn::LeftUp(float Val)
{
	LeftControl.Y = Val;
}

void AAsteroidsVRPawn::LeftRight(float Val)
{
	LeftControl.X = Val;
}

void AAsteroidsVRPawn::RightUp(float Val)
{
	RightControl.Y = Val;
}

void AAsteroidsVRPawn::RightRight(float Val)
{
	RightControl.X = Val;
}