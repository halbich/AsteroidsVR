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
	}

}


