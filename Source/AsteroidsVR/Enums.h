// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EKinectMeasureEnum : uint8
{
	Null				UMETA(DisplayName = "NULL"),
	BaseOfSpine			UMETA(DisplayName = "BaseOfSpine"),
	MiddlefSpine		UMETA(DisplayName = "MiddlefSpine"),
	Neck				UMETA(DisplayName = "Neck"),
	Head				UMETA(DisplayName = "Head"),
	LeftShoulder		UMETA(DisplayName = "LeftShoulder"),
	LeftElbow			UMETA(DisplayName = "LeftElbow"),
	LeftWrist			UMETA(DisplayName = "LeftWrist"),
	LeftHand			UMETA(DisplayName = "LeftHand"),
	RightShoulder		UMETA(DisplayName = "RightShoulder"),
	RightElbow			UMETA(DisplayName = "RightElbow"),
	RightWrist			UMETA(DisplayName = "RightWrist"),
	RightHand			UMETA(DisplayName = "RightHand")

};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EKinectMeasurePoseEnum : uint8
{
	Null			UMETA(DisplayName = "NULL"),
	Front			UMETA(DisplayName = "Front"),
	Top				UMETA(DisplayName = "Top"),
	Bottom			UMETA(DisplayName = "Bottom"),
	Right			UMETA(DisplayName = "Right"),
	Left			UMETA(DisplayName = "Left"),

};