// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enums.h"
#include "Components/ActorComponent.h"
#include "FCustomKinectMeasure.h"
#include "FHandConfigHelper.h"
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

	UFUNCTION(BlueprintCallable, Category = "Kinect | Setup")
		void FinalizeConfiguration();

	UPROPERTY()
		FVector2D LeftHand;

	UPROPERTY()
		FVector2D RightHand;

	UFUNCTION(BlueprintCallable, Category = "Kinect | Setup")
		void UpdateGamePose(FCustomKinectMeasure measure);
private:

	UPROPERTY()
		FCustomKinectMeasure PoseFront;
	UPROPERTY()
		FCustomKinectMeasure PoseTop;
	UPROPERTY()
		FCustomKinectMeasure PoseBottom;
	UPROPERTY()
		FCustomKinectMeasure PoseRight;
	UPROPERTY()
		FCustomKinectMeasure PoseLeft;


	UPROPERTY()
		FCustomKinectMeasure FinalNeutralPose;

	UPROPERTY()
		FHandConfigHelper RightHandHelper;

	UPROPERTY()
		FHandConfigHelper LeftHandHelper;
};
