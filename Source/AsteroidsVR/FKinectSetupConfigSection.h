// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FKinectSetupConfigSection.Generated.h"




/**
 *
 */

USTRUCT(BlueprintType)
struct ASTEROIDSVR_API FKinectSetupConfigSection
{
	GENERATED_BODY();



	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		FText MeasureText;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		float MeasureDuration;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		bool ShowProgressBar;

	UPROPERTY(BlueprintReadWrite, Category = "Kinect | Measures")
		EKinectMeasurePoseEnum MeasureType;

	FKinectSetupConfigSection()
	{
		MeasureText = FText::GetEmpty();
		MeasureDuration = .30f;
		ShowProgressBar = false;
		MeasureType = EKinectMeasurePoseEnum::Null;
	}

	FKinectSetupConfigSection(FText text) : FKinectSetupConfigSection()
	{
		MeasureText = text;
		MeasureDuration = .20f;
	}


	FKinectSetupConfigSection(FText text, EKinectMeasurePoseEnum type) : FKinectSetupConfigSection()
	{
		MeasureText = text;
		ShowProgressBar = true;
		MeasureType = type;
	}




};


