// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsVR.h"
#include "MyBPFunctionLibrary.h"

UMyBPFunctionLibrary::UMyBPFunctionLibrary(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}


void UMyBPFunctionLibrary::UpdateValue(FCustomKinectMeasure& measure, FVector value, EKinectMeasureEnum type)
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

void UMyBPFunctionLibrary::DebugShowHand(FVector origin, FRotator rotator, UPARAM(ref)FHandConfigHelper& hand, UWorld* world)
{
	if (!world)
		return;

	//UKismetSystemLibrary::DrawDebugArrow

}