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

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("front"))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("front")), EKinectMeasurePoseEnum::Front));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("top"))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("top")), EKinectMeasurePoseEnum::Top));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("bottom"))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("bottom")), EKinectMeasurePoseEnum::Bottom));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("right"))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("right")), EKinectMeasurePoseEnum::Right));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("left"))));
	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("left")), EKinectMeasurePoseEnum::Left));

	result.Add(FKinectSetupConfigSection(FText::FromString(TEXT("jedeme"))));

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
		OutRotationAngleDegrees = FMath::RadiansToDegrees(acosf(FVector2D::DotProduct(FVector2D(0,-1), OutScreenPosition.GetSafeNormal()))) * FMath::Sign(OutScreenPosition.X);
	}
}



#pragma optimize("", on)