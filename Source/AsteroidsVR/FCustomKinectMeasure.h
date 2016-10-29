// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FCustomKinectMeasure.Generated.h"

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


/**
 *
 */

USTRUCT(BlueprintType)
struct ASTEROIDSVR_API FCustomKinectMeasure
{
	GENERATED_BODY();



	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector BaseOfSpine;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector MiddlefSpine;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector Neck;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector Head;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector LeftShoulder;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector LeftElbow;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector LeftWrist;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector LeftHand;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector RightShoulder;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector RightElbow;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector RightWrist;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FVector RightHand;

	//UFUNCTION(BlueprintCallable, Category = "Kinect | Measures")
	void UpdateValue(FVector value, EKinectMeasureEnum type);

	FCustomKinectMeasure()
	{
		BaseOfSpine = FVector::ZeroVector;
		MiddlefSpine = FVector::ZeroVector;
		Neck = FVector::ZeroVector;
		Head = FVector::ZeroVector;
		LeftShoulder = FVector::ZeroVector;
		LeftElbow = FVector::ZeroVector;
		LeftWrist = FVector::ZeroVector;
		LeftHand = FVector::ZeroVector;
		RightShoulder = FVector::ZeroVector;
		RightElbow = FVector::ZeroVector;
		RightWrist = FVector::ZeroVector;
		RightHand = FVector::ZeroVector;
	}
};


