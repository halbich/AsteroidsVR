// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FHandConfigHelper.generated.h"

/**
 *
 */
USTRUCT()
struct ASTEROIDSVR_API FHandConfigHelper
{
	GENERATED_BODY();

	UPROPERTY()
		FVector Center;

	UPROPERTY()
		FVector Up;

	UPROPERTY()
		float UpDistance;

	UPROPERTY()
		FVector Down;

	UPROPERTY()
		float DownDistance;

	UPROPERTY()
		FVector Right;

	UPROPERTY()
		float RightDistance;

	UPROPERTY()
		FVector Left;

	UPROPERTY()
		float LeftDistance;


	FHandConfigHelper() {
		Center = FVector::ZeroVector;
		Up = FVector::ZeroVector;
		Down = FVector::ZeroVector;
		Right = FVector::ZeroVector;
		Left = FVector::ZeroVector;
		UpDistance = 0;
		DownDistance = 0;
		RightDistance = 0;
		LeftDistance = 0;

	}

	FHandConfigHelper(FVector center, FVector up, FVector down, FVector right, FVector left) : FHandConfigHelper()
	{
		Center = center;

		Up = up;
		UpDistance = FVector::Dist(Up, Center);

		Down = down;
		DownDistance = FVector::Dist(Down, Center);

		Right = right;
		RightDistance = FVector::Dist(Right, Center);

		Left = left;
		LeftDistance = FVector::Dist(Left, Center);
	}


	FVector2D GetDistance(const FVector& d)
	{

		auto dc = FVector::Dist(d, Center);

		auto res = getCoef(d, Up, dc, UpDistance) * FVector2D(0, 1);

		res += getCoef(d, Down, dc, DownDistance) * FVector2D(0, -1);

		res += getCoef(d, Right, dc, RightDistance) * FVector2D(1, 0);

		res += getCoef(d, Left, dc, LeftDistance) * FVector2D(-1, 0);

		if (res.Size() > 1)
			res.Normalize();

		return res;

	}

private:

	// D sm�r, T c�l, dc vzd�lenost sm�ru od st�edu, tc vzd�lenost c�le od st�edu
	FORCEINLINE float getCoef(const FVector& D, const FVector& T, const float& dc, const float& tc)
	{
		auto d = FVector::Dist(D, T);		// vzd�lenost sm�ru od c�le
		auto dMin = dc <= tc ? FMath::Min(d, tc) : 0;		// vzd�lenost omez�me na nejv��e vzd�lenost od st�edu
		return 1 - (dMin / tc);			// s jak�m koeficientem jsem naho�e

	}

};
