// Fill out your copyright notice in the Description page of Project Settings.

#include "KinectModule.h"
#include "KinectBodyData.h"


#pragma optimize("",off)

UKinectBodyData::UKinectBodyData()
{

}

void UKinectBodyData::UpdateJoint(EKinectJointType Joint, FVector Location, EKinectJointTrackingState TrackingState)
{
	FJointValueStruct& val = Joints.FindOrAdd(Joint);

	val.Location = Location;
	val.TrackingState = TrackingState;
}

FVector UKinectBodyData::GetJointLocation(EKinectJointType Joint, EKinectJointTrackingState& TrackingState)
{
	FJointValueStruct& val = Joints.FindOrAdd(Joint);

	TrackingState = val.TrackingState;
	return val.Location;
}


int32 UKinectBodyData::GetCenteringContant(bool& IsRelevant)
{
	FVector bodyPosition;
	int32 relevants = 0;

	bodyPosition += getRelevantCenteringJoint(EKinectJointType::Head, relevants);
	bodyPosition += getRelevantCenteringJoint(EKinectJointType::Neck, relevants);
	bodyPosition += getRelevantCenteringJoint(EKinectJointType::SpineMid, relevants);
	bodyPosition += getRelevantCenteringJoint(EKinectJointType::SpineBase, relevants);

	IsRelevant = relevants != 0;

	if (IsRelevant)
		bodyPosition /= relevants;

	return bodyPosition.Y;
}

#pragma optimize("",on)