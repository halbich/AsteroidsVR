// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma optimize("", off)

#include "FHandConfigHelper.generated.h"

#define SMALL_NUMBER		(1.e-8f)

struct ASTEROIDSVR_API FEffector
{
	FVector EffectDirection;
	FVector2D EffectResult;
	float CurrDistFromDir;

	FEffector(const FVector& direction, const  FVector2D& result, const FVector& currentDirection)
	{
		EffectDirection = direction;
		EffectResult = result;
		CurrDistFromDir = FMath::RadiansToDegrees(acosf(FVector::DotProduct(currentDirection.GetSafeNormal(), direction.GetSafeNormal())));

		auto ratio = CurrDistFromDir / 180.0f;
		auto resRatio = FMath::InterpStep(1.0f, -1.0f, ratio, 20);
		auto rr = FMath::Lerp(1.0f, -1.0f, ratio);

		EffectResult = result * rr;
	}
};


/**
 *
 */
USTRUCT()
struct ASTEROIDSVR_API FHandConfigHelper
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector Center;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector Up;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector UpProjection;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector Down;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector DownProjection;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector Right;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector RightProjection;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector Left;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector LeftProjection;

	UPROPERTY()
		float MeanLength;


	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector Closest;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector Closest2;


	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector Ax1;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector Ax2;

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FVector AxRes;

	FHandConfigHelper() {
		Center = FVector::ZeroVector;
		Up = FVector::ZeroVector;
		Down = FVector::ZeroVector;
		Right = FVector::ZeroVector;
		Left = FVector::ZeroVector;
		UpProjection = FVector::ZeroVector;
		DownProjection = FVector::ZeroVector;
		RightProjection = FVector::ZeroVector;
		LeftProjection = FVector::ZeroVector;

		MeanLength = 0;
		Closest = FVector::ZeroVector;
		Closest2 = FVector::ZeroVector;

		Ax1 = FVector::ZeroVector;
		Ax2 = FVector::ZeroVector;
		AxRes = FVector::ZeroVector;
	}

	FHandConfigHelper(const FVector& center, const FVector&  up, const FVector&  down, const FVector&  right, const FVector&  left) : FHandConfigHelper()
	{
		MeanLength = (center.Size() + up.Size() + down.Size() + right.Size() + left.Size()) / 5.0f;

		Center = center;
		Center.Normalize();
		Center *= MeanLength;

		Up = up;
		Up.Normalize();
		Up *= MeanLength;
		UpProjection = FVector::VectorPlaneProject(Up, Center.GetSafeNormal());

		Down = down;
		Down.Normalize();
		Down *= MeanLength;
		DownProjection = FVector::VectorPlaneProject(Down, Center.GetSafeNormal());

		Right = right;
		Right.Normalize();
		Right *= MeanLength;
		RightProjection = FVector::VectorPlaneProject(Right, Center.GetSafeNormal());

		Left = left;
		Left.Normalize();
		Left *= MeanLength;
		LeftProjection = FVector::VectorPlaneProject(Left, Center.GetSafeNormal());


	}


	FVector2D GetDistance(const FVector& d)
	{
		auto pn = d; //FVector::VectorPlaneProject(d, Center.GetSafeNormal());

		Closest = pn;

		FEffector top(Up/*Projection*/, FVector2D(0, 1), pn);
		FEffector bottom(Down/*Projection*/, FVector2D(0, -1), pn);
		FEffector right(Right/*Projection*/, FVector2D(1, 0), pn);
		FEffector left(Left/*Projection*/, FVector2D(-1, 0), pn);

		FVector2D res = top.EffectResult + right.EffectResult + bottom.EffectResult + left.EffectResult;

		res = res.ClampAxes(-1, 1);
	
		return res;

	}
};
#pragma optimize("", on)
