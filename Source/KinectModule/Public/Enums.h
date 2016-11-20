// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


UENUM(BlueprintType)
enum class EKinectBody : uint8
{
	Body0			UMETA(DisplayName = "Body 0"),
	Body1			UMETA(DisplayName = "Body 1"),
	Body2			UMETA(DisplayName = "Body 2"),
	Body3			UMETA(DisplayName = "Body 3"),
	Body4			UMETA(DisplayName = "Body 4"),
	Body5			UMETA(DisplayName = "Body 5")

};






UENUM(BlueprintType)
enum class EKinectJointType : uint8
{
	SpineBase 			UMETA(DisplayName = "SpineBase"),
	SpineMid 			UMETA(DisplayName = "SpineMid"),
	Neck 				UMETA(DisplayName = "Neck"),
	Head 				UMETA(DisplayName = "Head"),
	ShoulderLeft 		UMETA(DisplayName = "ShoulderLeft"),
	ElbowLeft 			UMETA(DisplayName = "ElbowLeft"),
	WristLeft 			UMETA(DisplayName = "WristLeft"),
	HandLeft 			UMETA(DisplayName = "HandLeft"),
	ShoulderRight 		UMETA(DisplayName = "ShoulderRight"),
	ElbowRight 			UMETA(DisplayName = "ElbowRight"),
	WristRight 			UMETA(DisplayName = "WristRight"),
	HandRight 			UMETA(DisplayName = "HandRight"),
	HipLeft 			UMETA(DisplayName = "HipLeft"),
	KneeLeft 			UMETA(DisplayName = "KneeLeft"),
	AnkleLeft 			UMETA(DisplayName = "AnkleLeft"),
	FootLeft 			UMETA(DisplayName = "FootLeft"),
	HipRight 			UMETA(DisplayName = "HipRight"),
	KneeRight 			UMETA(DisplayName = "KneeRight"),
	AnkleRight 			UMETA(DisplayName = "AnkleRight"),
	FootRight 			UMETA(DisplayName = "FootRight"),
	SpineShoulder 		UMETA(DisplayName = "SpineShoulder"),
	HandTipLeft 		UMETA(DisplayName = "HandTipLeft"),
	ThumbLeft 			UMETA(DisplayName = "ThumbLeft"),
	HandTipRight 		UMETA(DisplayName = "HandTipRight"),
	ThumbRight 			UMETA(DisplayName = "ThumbRight")

};


UENUM(BlueprintType)
enum class EKinectHand : uint8
{
	Left					UMETA(DisplayName = "Left"),
	Right					UMETA(DisplayName = "Right")
};




UENUM(BlueprintType)
enum class EKinectHandState : uint8
{
	Unknown					UMETA(DisplayName = "Unknown"),
	NotTracked				UMETA(DisplayName = "NotTracked"),
	Open					UMETA(DisplayName = "Open"),
	Closed					UMETA(DisplayName = "Closed"),
	Lasso					UMETA(DisplayName = "Lasso")
};

UENUM(BlueprintType)
enum class EKinectJointTrackingState : uint8
{
	NotTracked				UMETA(DisplayName = "NotTracked"),
	Inferred				UMETA(DisplayName = "Inferred"),
	Tracked					UMETA(DisplayName = "Tracked")
};


