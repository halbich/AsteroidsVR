// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsVR.h"
#include "FCustomKinectMeasure.h"



void FCustomKinectMeasure::UpdateValue(FVector value, EKinectMeasureEnum type)
{
	switch (type)
	{
	case EKinectMeasureEnum::BaseOfSpine: {BaseOfSpine = value; break; }
	case EKinectMeasureEnum::MiddlefSpine: {MiddlefSpine = value; break; }
	case EKinectMeasureEnum::Neck: {Neck = value; break; }
	case EKinectMeasureEnum::Head: {Head = value; break; }
	case EKinectMeasureEnum::LeftShoulder: {LeftShoulder = value; break; }
	case EKinectMeasureEnum::LeftElbow: {LeftElbow = value; break; }
	case EKinectMeasureEnum::LeftWrist: {LeftWrist = value; break; }
	case EKinectMeasureEnum::LeftHand: {LeftHand = value; break; }
	case EKinectMeasureEnum::RightShoulder: {RightShoulder = value; break; }
	case EKinectMeasureEnum::RightElbow: {RightElbow = value; break; }
	case EKinectMeasureEnum::RightWrist: {RightWrist = value; break; }
	case EKinectMeasureEnum::RightHand: {RightHand = value; break; }
	}

}