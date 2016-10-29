// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enums.h"
#include "Components/ActorComponent.h"
#include "FCustomKinectMeasure.h"
#include "KinectSetupComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ASTEROIDSVR_API UKinectSetupComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKinectSetupComponent();

	// Called when the game starts
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable, Category = "Kinect | Setup")
		void NotePose(FCustomKinectMeasure measure, EKinectMeasurePoseEnum type);


private:

	FCustomKinectMeasure PoseFront;

};
