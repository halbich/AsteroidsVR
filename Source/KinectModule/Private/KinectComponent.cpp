// Fill out your copyright notice in the Description page of Project Settings.

#include "KinectModule.h"
#include "KinectComponent.h"

#pragma optimize("", on)

// Sets default values for this component's properties
UKinectComponent::UKinectComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	m_pKinectSensor = NULL;
	m_pCoordinateMapper = NULL;


	m_pBodyFrameReader = NULL;

	frameHandle = NULL;
	aviableHandle = NULL;




}


// Called when the game starts
void UKinectComponent::BeginPlay()
{
	Super::BeginPlay();


	for (uint8 i = 0; i < BODY_COUNT; i++)
	{
		trackedBodies.Add((EKinectBody)i, NewObject<UKinectBodyData>(this));
	}

	HRESULT hr;


	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		UE_LOG(LogKinectModule, Error, TEXT("Failed to get Kinect sensor!"));
		return;
	}

	if (m_pKinectSensor)
	{

		BOOLEAN isAviable = false;
		auto isAvHr = m_pKinectSensor->get_IsAvailable(&isAviable);
		IsKinectAviable = SUCCEEDED(isAvHr) && isAviable;
		m_pKinectSensor->SubscribeIsAvailableChanged(&aviableHandle);


		// Initialize the Kinect and get coordinate mapper and the body reader
		IBodyFrameSource* pBodyFrameSource = NULL;

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);

		}

		if (SUCCEEDED(hr))
		{
			m_pBodyFrameReader->SubscribeFrameArrived(&frameHandle);
		}

		SafeRelease(pBodyFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		UE_LOG(LogKinectModule, Error, TEXT("No ready Kinect found!"));
		return;
	}


}

void UKinectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (m_pBodyFrameReader && frameHandle)
	{
		m_pBodyFrameReader->UnsubscribeFrameArrived(frameHandle);
		frameHandle = NULL;
	}

	// done with body frame reader
	SafeRelease(m_pBodyFrameReader);

	// done with coordinate mapper
	SafeRelease(m_pCoordinateMapper);

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->UnsubscribeIsAvailableChanged(aviableHandle);
		aviableHandle = NULL;

		m_pKinectSensor->Close();
	}

	SafeRelease(m_pKinectSensor);

	trackedBodies.Empty();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UKinectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{


	updateKinectAviable();

	updateKinectBodyData();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}


void UKinectComponent::updateKinectAviable()
{
	if (!aviableHandle)
		return;

	IIsAvailableChangedEventArgs* aviableChangedEventArgs = NULL;
	auto hr = m_pKinectSensor->GetIsAvailableChangedEventData(aviableHandle, &aviableChangedEventArgs);
	if (SUCCEEDED(hr))
	{
		BOOLEAN isAviable = false;
		hr = aviableChangedEventArgs->get_IsAvailable(&isAviable);
		IsKinectAviable = SUCCEEDED(hr) && isAviable;
	}

	SafeRelease(aviableChangedEventArgs);

}

void UKinectComponent::updateKinectBodyData()
{
	if (!frameHandle)
		return;

	IBodyFrameArrivedEventArgs* args = NULL;
	auto hr = m_pBodyFrameReader->GetFrameArrivedEventData(frameHandle, &args);


	if (SUCCEEDED(hr))
	{
		IBodyFrameReference* bodyRef = NULL;
		hr = args->get_FrameReference(&bodyRef);

		if (SUCCEEDED(hr) && bodyRef)
		{
			IBodyFrame* bodyFrame = NULL;
			hr = bodyRef->AcquireFrame(&bodyFrame);

			if (SUCCEEDED(hr) && bodyFrame)
			{
				processBodyFrame(bodyFrame);

			}
			SafeRelease(bodyFrame);
		}
		SafeRelease(bodyRef);
	}

	SafeRelease(args);


}



void UKinectComponent::processBodyFrame(IBodyFrame* bodyFrame)
{

	IBody* ppBodies[BODY_COUNT] = { 0 };

	auto hr = bodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);

	if (SUCCEEDED(hr))
	{
		processBodies(BODY_COUNT, ppBodies);
	}

	for (int32 i = 0; i < _countof(ppBodies); ++i)
	{
		SafeRelease(ppBodies[i]);
	}
}

void UKinectComponent::processBodies(int32 nBodyCount, IBody** ppBodies)
{

	if (!m_pCoordinateMapper)
		return;


	for (int32 i = 0; i < nBodyCount; ++i)
	{
		auto data = trackedBodies[(EKinectBody)i];
		ensure(data);

		IBody* pBody = ppBodies[i];
		if (!pBody)
			continue;

		BOOLEAN bTracked = false;
		auto hr = pBody->get_IsTracked(&bTracked);

		auto tracked = SUCCEEDED(hr) && bTracked;

		data->IsTracked = tracked;

		if (!tracked)
			continue;


		HandState leftHandState = HandState_Unknown;
		HandState rightHandState = HandState_Unknown;

		pBody->get_HandLeftState(&leftHandState);
		data->LeftHandState = (EKinectHandState)leftHandState;

		pBody->get_HandRightState(&rightHandState);
		data->RightHandState = (EKinectHandState)rightHandState;


		Joint joints[JointType_Count];
		hr = pBody->GetJoints(_countof(joints), joints);
		if (SUCCEEDED(hr))
		{
			for (int32 j = 0; j < _countof(joints); ++j)
			{
				auto joint = joints[j];

				data->UpdateJoint((EKinectJointType)joint.JointType, getFromJoint(joint), (EKinectJointTrackingState)joint.TrackingState);
			}

		}
	}
}

bool UKinectComponent::IsBodyTracked(EKinectBody Body)
{
	auto data = trackedBodies[Body];

	if (!data)
		return false;

	return data->IsTracked;
}


EKinectHandState UKinectComponent::GetHandState(EKinectBody Body, EKinectHand Hand)
{
	auto data = trackedBodies[Body];

	if (!data)
		return EKinectHandState::Unknown;

	return Hand == EKinectHand::Left ? data->LeftHandState : data->RightHandState;
}

FVector UKinectComponent::GetJointLocation(EKinectBody Body, EKinectJointType Joint, EKinectJointTrackingState& TrackingState)
{
	auto data = trackedBodies[Body];

	if (!data)
	{
		TrackingState = EKinectJointTrackingState::NotTracked;
		return FVector::ZeroVector;
	}

	return data->GetJointLocation(Joint, TrackingState);
}

EKinectBody UKinectComponent::GetCenteredBody(bool& IsTracked)
{
	IsTracked = false;
	EKinectBody centered = EKinectBody::Body0;

	int32 lastCenteredKonstant = 1024 * 1024;

	for (auto bodyPair : trackedBodies)
	{
		auto bodyVal = bodyPair.Value;
		if (!bodyVal || !bodyVal->IsTracked)
			continue;


		bool isRelevant = false;
		auto currentConstant = bodyVal->GetCenteringContant(isRelevant);

		if (isRelevant && currentConstant < lastCenteredKonstant)
		{
			lastCenteredKonstant = currentConstant;
			centered = bodyPair.Key;
			IsTracked = true;
		}
	}

	return centered;
}

bool UKinectComponent::IsKinectAwake()
{
	return IsKinectAviable;
}

#pragma optimize("", off)