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
			: PlaneMesh(TEXT("StaticMesh'/Game/Geometry/Viper/viperMkII.viperMkII'"))
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
	
	if (ControlError)
	{
		Super::Tick(DeltaSeconds);
		return;
	}

	if (UseKinect && currentKinectConfig)
	{
		LeftControl = currentKinectConfig->LeftHand;
		RightControl = currentKinectConfig->RightHand;
	}

	if (UseControlSteps)
	{
		LeftControl = GridSnap(LeftControl);
		RightControl = GridSnap(RightControl);
	}

	auto left = LeftControl;
	auto right = RightControl;



	auto NewForwardSpeed = CurrentForwardSpeed;

	auto isRoll = left.Y * right.Y < 0;

	if (!isRoll)
	{
		CurrentPitchSpeed = (left.Y + right.Y) *0.5f;
	}
	else
	{
		CurrentRollSpeed = (left.Y + -1.0f*right.Y) * 0.5f;
	}

	

	auto isBrake = left.X * right.X < 0;

	if (!isBrake)
	{
		NewForwardSpeed += ContinualAcceleration * DeltaSeconds;
		CurrentYawSpeed = (left.X + right.X) *0.5f;
	}
	else
	{
		NewForwardSpeed += (left.X + -1.0f*right.X) * 0.5f * BrakeSpeed * DeltaSeconds;
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



void AAsteroidsVRPawn::LeftUp(float Val)
{
	if (UseKinect)
		return;

	LeftControl.Y = Val;
}

void AAsteroidsVRPawn::LeftRight(float Val)
{
	if (UseKinect)
		return;

	LeftControl.X = Val;
}

void AAsteroidsVRPawn::RightUp(float Val)
{
	if (UseKinect)
		return;
	RightControl.Y = Val;
}

void AAsteroidsVRPawn::RightRight(float Val)
{
	if (UseKinect)
		return;
	RightControl.X = Val;
}

void AAsteroidsVRPawn::SetNewUseKinect(bool newUseKinect)
{
	UseKinect = newUseKinect;

	currentKinectConfig = nullptr;

	if (!UseKinect)
		return;

	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!pc)
		return;



	auto sc = pc->GetComponentByClass(UKinectSetupComponent::StaticClass());

	if (sc)
		currentKinectConfig = Cast<UKinectSetupComponent>(sc);

}

void AAsteroidsVRPawn::RegisterSetupComponent(UKinectSetupComponent* kinectConfig)
{


}

void AAsteroidsVRPawn::BeginPlay()
{
	if (InvertY)
	{
		PitchSpeed *= -1;
	//	RollSpeed *= -1;
	}

	Super::BeginPlay();
}

void AAsteroidsVRPawn::SetNewInvertY(bool newInvertY)
{
	if (InvertY == newInvertY)
		return;

	InvertY = newInvertY;
	PitchSpeed *= -1;
}