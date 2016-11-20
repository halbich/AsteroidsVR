// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FCustomKinectMeasure.h"
#include "FKinectSetupConfigSection.h"
#include "FHandConfigHelper.h"
#include "KinectEnums.h"
#include "MyBPFunctionLibrary.generated.h"

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

	UFUNCTION(BlueprintCallable, Category = "AsteroidsBPLibrary")
		static void DebugShowHand(FVector origin, FRotator rotator, UPARAM(ref)FHandConfigHelper& hand, UWorld* world);


	/**
	* Converts a world location to screen position for HUD drawing. This differs from the results of FSceneView::WorldToScreen in that it returns a position along the edge of the screen for offscreen locations
	*
	* @param		InLocation	- The world space location to be converted to screen space
	* @param		EdgePercent - How close to the edge of the screen, 1.0 = at edge, 0.0 = at center of screen. .9 or .95 is usually desirable
	* @outparam	OutScreenPosition - the screen coordinates for HUD drawing
	* @outparam	OutRotationAngleDegrees - The angle to rotate a hud element if you want it pointing toward the offscreen indicator, 0° if onscreen
	* @outparam	bIsOnScreen - True if the specified location is in the camera view (may be obstructed)
	*/
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext), Category = "AsteroidsBPLibrary|Util")
		static void FindScreenEdgeLocationForWorldLocation(UObject* WorldContextObject, const FVector& InLocation, const float EdgePercent, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOnScreen);

	UFUNCTION(BlueprintPure, Category = "AsteroidsBPLibrary|Util", meta = (WorldContext = "WorldContextObject"))
		static void MapEdgeLoc(UObject* WorldContextObject, APlayerController* Controller, const FVector& ObjectLocation, const float EdgeBorder, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOutScreen, bool &bIsOutView);
private:

	FORCEINLINE static bool isOutRange(FVector2D& vect, FVector2D& border)
	{
		auto absV = vect.GetAbs();
		return absV.X > border.X || absV.Y > border.Y;
	}

#pragma optimize("",off)
	 static FVector2D CustomClamp(FVector2D& vect, FVector2D& border)
	{
		auto resVect = FVector2D(FMath::Clamp(vect.X, border.X *-1, border.X), FMath::Clamp(vect.Y, border.Y *-1, border.Y));

		auto rx = resVect.X / vect.X;

		auto ry = resVect.Y / vect.Y;

		if (rx != 1.0f && ry != 1.0f)
		{
			print(TEXT("not equal!"));
			print(*FText::AsNumber(rx).ToString());

			print(*FText::AsNumber(ry).ToString());

		}

		return resVect;
	}


#pragma optimize("",on)
};
