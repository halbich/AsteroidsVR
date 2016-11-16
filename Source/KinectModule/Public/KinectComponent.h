// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"

/*
	Using https://answers.unrealengine.com/questions/27560/trouble-using-windows-includes-with-dword-int.html 
	following Allow/Hide construction prevents ambigous symbols
*/

#include "AllowWindowsPlatformTypes.h" 
#include "Kinect.h"
#include "HideWindowsPlatformTypes.h"


#include "KinectComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINECTMODULE_API UKinectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKinectComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
