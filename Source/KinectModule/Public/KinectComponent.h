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

#include "KinectEnums.h"
#include "KinectBodyData.h"

#include "KinectComponent.generated.h"



// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KINECTMODULE_API UKinectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKinectComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	UFUNCTION(BlueprintCallable, Category = "Kinect")
		bool IsKinectAwake();

	UPROPERTY()
		bool IsKinectAviable;

	UFUNCTION(BlueprintCallable, Category = "Kinect")
		bool IsBodyTracked(EKinectBody Body);

	UPROPERTY()
		TMap<EKinectBody, UKinectBodyData*> trackedBodies;

	UFUNCTION(BlueprintCallable, Category = "Kinect")
		EKinectHandState GetHandState(EKinectBody Body, EKinectHand Hand);

	UFUNCTION(BlueprintCallable, Category = "Kinect")
		FVector GetJointLocation(EKinectBody Body, EKinectJointType Joint, EKinectJointTrackingState& TrackingState);

	UFUNCTION(BlueprintCallable, Category = "Kinect")
		EKinectBody GetCenteredBody(bool& IsTracked);

private:

	// Current Kinect

	IKinectSensor*          m_pKinectSensor;
	ICoordinateMapper*      m_pCoordinateMapper;

	// Body reader
	IBodyFrameReader*       m_pBodyFrameReader;


	WAITABLE_HANDLE aviableHandle;

	WAITABLE_HANDLE frameHandle;

	void updateKinectAviable();

	void updateKinectBodyData();

	void processBodyFrame(IBodyFrame* bodyFrame);

	void processBodies(int32 nBodyCount, IBody** ppBodies);


	static FVector getFromJoint(Joint j)
	{
		FVector res(-1 * j.Position.Z, j.Position.X, j.Position.Y);
		return res * 100;
	}
};
