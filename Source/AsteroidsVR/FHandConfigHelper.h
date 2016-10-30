// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
		CurrDistFromDir = FVector::Dist(currentDirection, direction);
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
		UpProjection = FVector::PointPlaneProject(Up, ProjectionPlane);

		Down = down;
		Down.Normalize();
		Down *= MeanLength;
		DownProjection = FVector::PointPlaneProject(Down, ProjectionPlane);

		Right = right;
		Right.Normalize();
		Right *= MeanLength;
		RightProjection = FVector::PointPlaneProject(Right, ProjectionPlane);

		Left = left;
		Left.Normalize();
		Left *= MeanLength;
		LeftProjection = FVector::PointPlaneProject(Left, ProjectionPlane);


	}








private:

	// D smìr, T cíl, dc vzdálenost smìru od støedu, tc vzdálenost cíle od støedu
	FORCEINLINE float getCoef(const FVector& D, const FVector& T, const float& dc, const float& tc)
	{
		auto d = FVector::Dist(D, T);		// vzdálenost smìru od cíle
		auto dMin = dc <= tc ? FMath::Min(d, tc) : 0;		// vzdálenost omezíme na nejvýše vzdálenost od støedu
		return 1 - (dMin / tc);			// s jakým koeficientem jsem nahoøe

	}


	FORCEINLINE void CmpEffs(FEffector& closest, FEffector& closest2, FEffector& test)
	{
		if (test.CurrDistFromDir < closest.CurrDistFromDir)	// testovaný je blíže než nejmenší
		{
			closest2 = closest;		// propaguji 2. místo
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


		FEffector c(Center, FVector2D(0, 0), _d);
		FEffector top(Up, FVector2D(0, 1), _d);
		FEffector right(Right, FVector2D(1, 0), _d);
		FEffector bottom(Down, FVector2D(0, -1), _d);
		FEffector left(Left, FVector2D(-1, 0), _d);


		auto isTopCls = top.CurrDistFromDir < right.CurrDistFromDir;
		auto closest = isTopCls ? top : right;
		auto closest2 = isTopCls ? right : top;

		CmpEffs(closest, closest2, bottom);
		CmpEffs(closest, closest2, left);


		Closest = closest.EffectDirection;
		Closest2 = closest2.EffectDirection;

		auto distance1 = closest.CurrDistFromDir;
		auto distance2 = closest2.CurrDistFromDir;

		auto p1 = FVector::VectorPlaneProject(closest.EffectDirection, Center.GetSafeNormal());
		auto p2 = FVector::VectorPlaneProject(closest2.EffectDirection, Center.GetSafeNormal());
		auto pn = FVector::VectorPlaneProject(_d, Center.GetSafeNormal());

		auto d1 = FVector::Dist(pn, p1);
		auto d2 = FVector::Dist(pn, p2);
		auto dt = FMath::Max(d1 + d2, SMALL_NUMBER);

		auto k1 = (1 - (d1 / dt));
		auto k2 = (1 - (d2 / dt));


		FVector2D res;
		res += k1* closest.EffectResult;
		res += k2 * closest2.EffectResult;



		Closest = p1;
		Closest2 = pn;


		if (res.Size() > 1)
			res.Normalize();

		return res;

	}

};
