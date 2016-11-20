// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Enums.h"
#include "KinectBodyData.generated.h"


USTRUCT()
struct FJointValueStruct {

	GENERATED_BODY();

	UPROPERTY()
		FVector Location;

	UPROPERTY()
		EKinectJointTrackingState TrackingState;


	FJointValueStruct() {
		Location = FVector::ZeroVector;
		TrackingState = EKinectJointTrackingState::NotTracked;
	};
};

/**
 *
 */
UCLASS()
class KINECTMODULE_API UKinectBodyData : public UObject
{
	GENERATED_BODY()

public:

	UKinectBodyData();

	UPROPERTY()
		bool IsTracked;

	UPROPERTY()
		EKinectHandState LeftHandState;

	UPROPERTY()
		EKinectHandState RightHandState;

	UPROPERTY()
		TMap<EKinectJointType, FJointValueStruct> Joints;

	void UpdateJoint(EKinectJointType Joint, FVector Location, EKinectJointTrackingState TrackingState);

	FVector GetJointLocation(EKinectJointType Joint, EKinectJointTrackingState& TrackingState);

	int32 GetCenteringContant(bool& IsRelevant);

private:

	FORCEINLINE FVector getRelevantCenteringJoint(EKinectJointType Joint, int32& relevants) {

		EKinectJointTrackingState trackState;
		auto res = GetJointLocation(Joint, trackState);

		if (trackState != EKinectJointTrackingState::NotTracked)
			++relevants;

		return res;
	}
};
