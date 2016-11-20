// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsVR.h"
#include "FCustomKinectMeasure.h"



void FCustomKinectMeasure::UpdateValue(FVector value, EKinectJointType type)
{
	switch (type)
	{
	case EKinectJointType::SpineBase: {BaseOfSpine = value; break; }
	case EKinectJointType::SpineMid: {MiddlefSpine = value; break; }
	case EKinectJointType::Neck: {Neck = value; break; }
	case EKinectJointType::Head: {Head = value; break; }
	case EKinectJointType::ShoulderLeft: {LeftShoulder = value; break; }
	case EKinectJointType::ElbowLeft: {LeftElbow = value; break; }
	case EKinectJointType::WristLeft: {LeftWrist = value; break; }
	case EKinectJointType::HandLeft: {LeftHand = value; break; }
	case EKinectJointType::ShoulderRight: {RightShoulder = value; break; }
	case EKinectJointType::ElbowRight: {RightElbow = value; break; }
	case EKinectJointType::WristRight: {RightWrist = value; break; }
	case EKinectJointType::HandRight: {RightHand = value; break; }
	}

}