// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FCustomKinectMeasure.h"
#include "MyBPFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class ASTEROIDSVR_API UMyBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

		UFUNCTION(BlueprintCallable, Category = "AsteroidsBPLibrary")
		static void UpdateValue(UPARAM(ref) FCustomKinectMeasure& measure, FVector value, EKinectMeasureEnum type);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AsteroidsBPLibrary")
		static FCustomKinectMeasure GetEmptyMeasure();
};
