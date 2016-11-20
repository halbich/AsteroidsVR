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
#pragma optimize("",on)