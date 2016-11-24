// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsVR.h"
#include "WidgetLayoutLibrary.h"
#include "MyBPFunctionLibrary.h"

UMyBPFunctionLibrary::UMyBPFunctionLibrary(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}


void UMyBPFunctionLibrary::UpdateValue(FCustomKinectMeasure& measure, FVector value, EKinectJointType type)
{
	measure.UpdateValue(value, type);
}

FCustomKinectMeasure UMyBPFunctionLibrary::GetEmptyMeasure() {
	FCustomKinectMeasure measure;
	return measure;
}

FCustomKinectMeasure UMyBPFunctionLibrary::GetMostCenteredPosition(UPARAM(ref) TArray<FCustomKinectMeasure>& measures)
{
	FCustomKinectMeasure measure;
	for (FCustomKinectMeasure& m : measures)
	{
		measure += m;
	}
	measure /= measures.Num();
	return measure;
}


TArray<FKinectSetupConfigSection> UMyBPFunctionLibrary::GetDefaultKinectConfigSetup()
{
	TArray<FKinectSetupConfigSection> result;

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("P�iprav se a dej ruce dop�edu. Takhle polet� rovn�."))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("M���m dop�edn� sm�r")), EKinectMeasurePoseEnum::Front));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("Nyn� dej ruce vzh�ru."))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("M���m. Takhle polet� nahoru.")), EKinectMeasurePoseEnum::Top));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("Nyn� dej ruce dol� pro opa�n� sm�r - zato�en� dol�."))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("M���m. Bude n�sledovat sm�r vpravo, ale a� ti �eknu, d��v ne!")), EKinectMeasurePoseEnum::Bottom));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("Tak te� dej ruce vpravo. Sna� se nevyt��et trup."))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("M���m. Tipne� si, jak� bude dal�� sm�r?")), EKinectMeasurePoseEnum::Right));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("Uh�dl jsi to, je to samoz�ejm� doleva."))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("M���m. Za chvilku budeme hotovi. Snad t� u� neza�aly bolet ruce?")), EKinectMeasurePoseEnum::Left));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("Tak a hur� na to, kapit�ne! Hodn� �t�st�."))));

	return result;
}



#pragma optimize("", off)

void UMyBPFunctionLibrary::MapEdgeLoc(UObject* WorldContextObject, APlayerController* Controller, const FVector& ObjectLocation, const float EdgeBorder, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOutScreen, bool &bIsOutView)
{
	bIsOutScreen = false;
	OutRotationAngleDegrees = 0.f;
	FVector2D ScreenPosition;

	bIsOutView = !UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(Controller, ObjectLocation, ScreenPosition);

	if (bIsOutView)
		return;

	auto vSize = UWidgetLayoutLibrary::GetViewportSize(WorldContextObject);
	auto vScale = UWidgetLayoutLibrary::GetViewportScale(WorldContextObject);

	auto resSize = vSize / vScale;
	auto border = resSize * 0.5;
	auto innerBorder = (resSize - FVector2D(EdgeBorder, EdgeBorder)) * 0.5;


	auto resultPos = ScreenPosition - border;

	bIsOutScreen = isOutRange(resultPos, innerBorder);
	OutScreenPosition = CustomClamp(resultPos, innerBorder);

	if (bIsOutScreen) {
		OutRotationAngleDegrees = FMath::RadiansToDegrees(acosf(FVector2D::DotProduct(FVector2D(0, -1), OutScreenPosition.GetSafeNormal()))) * FMath::Sign(OutScreenPosition.X);
	}
}


FVector UMyBPFunctionLibrary::CalculateTargetShootingPosition(const FVector& TargetLocation, const FVector& TargetForward, const float TargetSpeed, const FVector& BaseLocation, const float BaseShootSpeed)
{
	FTargetHelper left(TargetLocation, 0, FVector::Dist(TargetLocation, BaseLocation) / BaseShootSpeed);

	auto rightLoc = TargetLocation + left.ProjectileFlyTime * TargetForward * TargetSpeed;
	FTargetHelper right(rightLoc, left.ProjectileFlyTime, FVector::Dist(rightLoc, BaseLocation) / BaseShootSpeed);


	const int32 maxIterations = 20;

	for (size_t i = maxIterations; i > 0; --i)
	{
		if (right.IsOverShoot)
		{
			auto newHelpLoc = TargetLocation + right.ProjectileFlyTime * TargetForward * TargetSpeed;
			FTargetHelper newHelp(newHelpLoc, right.ProjectileFlyTime, FVector::Dist(newHelpLoc, BaseLocation) / BaseShootSpeed);

			if (newHelp.IsOverShoot)
				right = newHelp;
			else
			{
				left = newHelp;

				auto newRight = TargetLocation + left.ProjectileFlyTime * TargetForward * TargetSpeed;
				right = FTargetHelper(newRight, left.ProjectileFlyTime, FVector::Dist(newRight, BaseLocation) / BaseShootSpeed);
			}
		}
		else
		{
			left = right;
			auto newHelpLoc = TargetLocation + left.ProjectileFlyTime * TargetForward * TargetSpeed;
			right = FTargetHelper(newHelpLoc, left.ProjectileFlyTime, FVector::Dist(newHelpLoc, BaseLocation) / BaseShootSpeed);
		}
	}

	if (!right.IsOverShoot)
		return right.TargetLocation;


	return left.TargetLocation;
}


#pragma optimize("", on)