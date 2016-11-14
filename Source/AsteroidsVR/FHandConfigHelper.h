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

	UPROPERTY(BlueprintReadOnly, Category = "Kinect | Setup | HandConfig")
		FPlane ProjectionPlane;

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

		ProjectionPlane = FPlane();

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

		ProjectionPlane = FPlane(FVector::ZeroVector, Center);

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








private:

	// D sm�r, T c�l, dc vzd�lenost sm�ru od st�edu, tc vzd�lenost c�le od st�edu
	FORCEINLINE float getCoef(const FVector& D, const FVector& T, const float& dc, const float& tc)
	{
		auto d = FVector::Dist(D, T);		// vzd�lenost sm�ru od c�le
		auto dMin = dc <= tc ? FMath::Min(d, tc) : 0;		// vzd�lenost omez�me na nejv��e vzd�lenost od st�edu
		return 1 - (dMin / tc);			// s jak�m koeficientem jsem naho�e

	}


	FORCEINLINE void CmpEffs(FEffector& closest, FEffector& closest2, FEffector& test)
	{
		if (test.CurrDistFromDir < closest.CurrDistFromDir)	// testovan� je bl�e ne� nejmen��
		{
			closest2 = closest;		// propaguji 2. m�sto
			closest = test;
		}
		else
		{
			if (test.CurrDistFromDir < closest2.CurrDistFromDir)
				closest2 = test;
		}
	}

public:



	FVector2D GetDistance(const FVector& d)
	{


		auto _d = d;
		_d.Normalize();
		_d *= MeanLength;
		auto pn = FVector::VectorPlaneProject(_d, Center.GetSafeNormal());


		FEffector top(UpProjection, FVector2D(0, 1), pn);
		FEffector right(RightProjection, FVector2D(1, 0), pn);
		FEffector bottom(DownProjection, FVector2D(0, -1), pn);
		FEffector left(LeftProjection, FVector2D(-1, 0), pn);



		FVector2D res = top.EffectResult + right.EffectResult + bottom.EffectResult + left.EffectResult;

		if (res.Size() > 1)
			res.Normalize();

		print(*res.ToString());
		return res;

	}
};
#pragma optimize("", on)
