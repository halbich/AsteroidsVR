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

	UFUNCTION(BlueprintCallable, Category = "Kinect | Setup")
		void UpdateDummyGamePose(FCustomKinectMeasure measure);

	UPROPERTY()
		bool GenerateDummyPoses;
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

public:
	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandHelper")
		FHandConfigHelper RightHandHelper;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandHelper")
		FVector LastMeasuredRightHandDirection;

	UPROPERTY()
		FHandConfigHelper LeftHandHelper;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandHelper")
		FVector LastMeasuredLeftHandDirection;


private:

	FORCEINLINE void prepareMeasure(FCustomKinectMeasure& measure) {

		measure.MiddlefSpine = measure.BaseOfSpine + FVector(0, 0, 40);
		measure.Neck = measure.MiddlefSpine + FVector(0, 0, 40);
		measure.Head = measure.Neck + FVector(0, 0, 40);

		measure.LeftShoulder = (measure.Neck + measure.MiddlefSpine) / 2 + FVector(40, 0, 0);
		measure.RightShoulder = measure.LeftShoulder * -1;
	}

	FORCEINLINE void setMeasure(FCustomKinectMeasure& measure, const FVector& target) {

		prepareMeasure(measure);

		auto hand = target;
		auto wrist = target * 0.9;
		auto elbow = target * 0.5;

		measure.LeftElbow = measure.LeftShoulder + elbow;
		measure.LeftWrist = measure.LeftShoulder + wrist;
		measure.LeftHand = measure.LeftShoulder + hand;

		measure.RightElbow = measure.RightShoulder + elbow;
		measure.RightWrist = measure.RightShoulder + wrist;
		measure.RightHand = measure.RightShoulder + hand;
	}
};
