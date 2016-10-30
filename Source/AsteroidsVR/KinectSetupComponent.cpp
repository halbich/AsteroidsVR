// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsVR.h"
#include "KinectSetupComponent.h"


// Sets default values for this component's properties
UKinectSetupComponent::UKinectSetupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UKinectSetupComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UKinectSetupComponent::NotePose(FCustomKinectMeasure measure, EKinectMeasurePoseEnum type)
{
	switch (type) {

	case EKinectMeasurePoseEnum::Front: PoseFront = measure; break;
	case EKinectMeasurePoseEnum::Top: PoseTop = measure; break;
	case EKinectMeasurePoseEnum::Bottom: PoseBottom = measure; break;
	case EKinectMeasurePoseEnum::Right: PoseRight = measure; break;
	case EKinectMeasurePoseEnum::Left: PoseLeft = measure; break;
	}

}


void UKinectSetupComponent::UpdateGamePose(FCustomKinectMeasure measure)
{
	LastMeasuredRightHandDirection = measure.GetRightHandDirection();
	LastMeasuredRightHandDirection.Normalize();
	LastMeasuredRightHandDirection *= RightHandHelper.MeanLength;


	RightHand = RightHandHelper.GetDistance(LastMeasuredRightHandDirection);
	LeftHand = LeftHandHelper.GetDistance(measure.GetLeftHandDirection());

}

void UKinectSetupComponent::FinalizeConfiguration()
{

	print(TEXT("finalizuji"));

	FinalNeutralPose = (PoseFront + PoseTop + PoseBottom + PoseRight + PoseLeft);
	FinalNeutralPose /= 5;

	RightHandHelper = FHandConfigHelper(
		PoseFront.GetRightHandDirection(),
		PoseTop.GetRightHandDirection(),
		PoseBottom.GetRightHandDirection(),
		PoseRight.GetRightHandDirection(),
		PoseLeft.GetRightHandDirection()
	);

	LeftHandHelper = FHandConfigHelper(
		PoseFront.GetLeftHandDirection(),
		PoseTop.GetLeftHandDirection(),
		PoseBottom.GetLeftHandDirection(),
		PoseLeft.GetLeftHandDirection(),
		PoseLeft.GetLeftHandDirection()
	);

}