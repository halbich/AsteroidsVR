// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsVR.h"
#include "MyBPFunctionLibrary.h"

UMyBPFunctionLibrary::UMyBPFunctionLibrary(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

//Happy Message
void UMyBPFunctionLibrary::UpdateValue(FCustomKinectMeasure& measure, FVector value, EKinectMeasureEnum type)
{
	measure.UpdateValue(value, type);
}

FCustomKinectMeasure UMyBPFunctionLibrary::GetEmptyMeasure() {
	FCustomKinectMeasure measure;
	return measure;
}
