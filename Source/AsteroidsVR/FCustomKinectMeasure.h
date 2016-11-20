// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KinectEnums.h"
#include "FCustomKinectMeasure.generated.h"




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

	void UpdateValue(FVector value, EKinectJointType type);

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


	FCustomKinectMeasure& operator+=(const FCustomKinectMeasure& rhs)		// compound assignment (does not need to be a member,
	{																		// but often is, to modify the private members)

		BaseOfSpine += rhs.BaseOfSpine;
		MiddlefSpine += rhs.MiddlefSpine;
		Neck += rhs.Neck;
		Head += rhs.Head;
		LeftShoulder += rhs.LeftShoulder;
		LeftElbow += rhs.LeftElbow;
		LeftWrist += rhs.LeftWrist;
		LeftHand += rhs.LeftHand;
		RightShoulder += rhs.RightShoulder;
		RightElbow += rhs.RightElbow;
		RightWrist += rhs.RightWrist;
		RightHand += rhs.RightHand;

		return *this; // return the result by reference
	}

	// friends defined inside class body are inline and are hidden from non-ADL lookup
	friend FCustomKinectMeasure operator+(FCustomKinectMeasure lhs,			// passing lhs by value helps optimize chained a+b+c
		const FCustomKinectMeasure& rhs)									// otherwise, both parameters may be const references
	{
		lhs += rhs; // reuse compound assignment
		return lhs; // return the result by value (uses move constructor)
	}

	FCustomKinectMeasure& operator/=(const float& rhs)		// compound assignment (does not need to be a member,
	{																		// but often is, to modify the private members)

		BaseOfSpine /= rhs;
		MiddlefSpine /= rhs;
		Neck /= rhs;
		Head /= rhs;
		LeftShoulder /= rhs;
		LeftElbow /= rhs;
		LeftWrist /= rhs;
		LeftHand /= rhs;
		RightShoulder /= rhs;
		RightElbow /= rhs;
		RightWrist /= rhs;
		RightHand /= rhs;

		return *this; // return the result by reference
	}

	// friends defined inside class body are inline and are hidden from non-ADL lookup
	friend FCustomKinectMeasure operator+(FCustomKinectMeasure lhs,			// passing lhs by value helps optimize chained a+b+c
		const float& rhs)									// otherwise, both parameters may be const references
	{
		lhs /= rhs; // reuse compound assignment
		return lhs; // return the result by value (uses move constructor)
	}

	FVector GetRightHandDirection()
	{
		return getTotalDirection(RightShoulder, RightElbow, RightWrist, RightHand);
	}

	FVector GetLeftHandDirection()
	{
		return getTotalDirection(LeftShoulder, LeftElbow, LeftWrist, LeftHand);
	}


private:
	FORCEINLINE FVector getTotalDirection(const FVector& shoulder,
		const FVector& elbow,
		const FVector& wrist,
		const FVector& hand)
	{
		auto v1 = elbow - shoulder;
		auto v2 = wrist - shoulder;
		auto v3 = hand - shoulder;

		auto result = (v1 + v2 + v3) / 3.0f;
		result.Normalize();

		auto handDist = FVector::Dist(hand, shoulder);
		return result * handDist;

	}

};


