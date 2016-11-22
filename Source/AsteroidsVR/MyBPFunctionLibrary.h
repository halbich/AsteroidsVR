// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FCustomKinectMeasure.h"
#include "FKinectSetupConfigSection.h"
#include "FHandConfigHelper.h"
#include "KinectEnums.h"
#include "MyBPFunctionLibrary.generated.h"


struct FTargetHelper
{
	FVector TargetLocation;
	float TargetFlyTime;
	float ProjectileFlyTime;

	/* ProjectileFlyTime < TargetFlyTime, tedy projektil bude v místì døíve */
	bool IsOverShoot;

	FTargetHelper(FVector targetLocation, float targetFlyTime, float projectileFlyTime)
	{
		TargetLocation = targetLocation;
		TargetFlyTime = targetFlyTime;
		ProjectileFlyTime = projectileFlyTime;
		IsOverShoot = ProjectileFlyTime < TargetFlyTime;
	}
};
/**
 *
 */
UCLASS()
class ASTEROIDSVR_API UMyBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

		UFUNCTION(BlueprintCallable, Category = "AsteroidsBPLibrary")
		static void UpdateValue(UPARAM(ref) FCustomKinectMeasure& measure, FVector value, EKinectJointType type);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AsteroidsBPLibrary")
		static FCustomKinectMeasure GetEmptyMeasure();

	UFUNCTION(BlueprintCallable, Category = "AsteroidsBPLibrary")
		static FCustomKinectMeasure GetMostCenteredPosition(UPARAM(ref) TArray<FCustomKinectMeasure>& measures);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AsteroidsBPLibrary")
		static TArray<FKinectSetupConfigSection> GetDefaultKinectConfigSetup();


	UFUNCTION(BlueprintPure, Category = "AsteroidsBPLibrary|Util", meta = (WorldContext = "WorldContextObject"))
		static void MapEdgeLoc(UObject* WorldContextObject, APlayerController* Controller, const FVector& ObjectLocation, const float EdgeBorder, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOutScreen, bool &bIsOutView);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AsteroidsBPLibrary")
		static FVector CalculateTargetShootingPosition(const FVector& TargetLocation, const FVector& TargetForward, const float TargetSpeed, const FVector& BaseLocation, const float BaseShootSpeed);

private:

	FORCEINLINE static bool isOutRange(FVector2D& vect, FVector2D& border)
	{
		auto absV = vect.GetAbs();
		return absV.X > border.X || absV.Y > border.Y;
	}


	static FVector2D CustomClamp(const FVector2D& vect, const FVector2D& border)
	{
		auto res = FVector2D(vect);

		if (res.X < border.X *-1 || res.X > border.X)
		{
			res.Y = (res.Y / res.X) * border.X * FMath::Sign(res.X);
			res.X = border.X * FMath::Sign(res.X);
		}

		if (res.Y < border.Y *-1 || res.Y > border.Y)
		{
			res.X = (res.X / res.Y) * border.Y * FMath::Sign(res.Y);
			res.Y = border.Y * FMath::Sign(res.Y);
		}

		return res;
	}

};
