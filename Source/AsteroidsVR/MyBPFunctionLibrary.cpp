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

void UMyBPFunctionLibrary::DebugShowHand(FVector origin, FRotator rotator, UPARAM(ref)FHandConfigHelper& hand, UWorld* world)
{
	if (!world)
		return;

	//UKismetSystemLibrary::DrawDebugArrow

}

void UMyBPFunctionLibrary::FindScreenEdgeLocationForWorldLocation(UObject* WorldContextObject, const FVector& InLocation, const float EdgePercent, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOnScreen)
{
	bIsOnScreen = false;
	OutRotationAngleDegrees = 0.f;
	FVector2D *ScreenPosition = new FVector2D();

	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);

	if (!World) return;

	APlayerController* PlayerController = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : NULL);
	ACharacter *PlayerCharacter = static_cast<ACharacter *> (PlayerController->GetPawn());

	if (!PlayerCharacter) return;


	FVector Forward = PlayerCharacter->GetActorForwardVector();
	FVector Offset = (InLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();

	float DotProduct = FVector::DotProduct(Forward, Offset);
	bool bLocationIsBehindCamera = (DotProduct < 0);

	if (bLocationIsBehindCamera)
	{
		// For behind the camera situation, we cheat a little to put the
		// marker at the bottom of the screen so that it moves smoothly
		// as you turn around. Could stand some refinement, but results
		// are decent enough for most purposes.

		FVector DiffVector = InLocation - PlayerCharacter->GetActorLocation();
		FVector Inverted = DiffVector * -1.f;
		FVector NewInLocation = PlayerCharacter->GetActorLocation() * Inverted;

		NewInLocation.Z -= 5000;

		PlayerController->ProjectWorldLocationToScreen(NewInLocation, *ScreenPosition);
		ScreenPosition->Y = (EdgePercent * ViewportCenter.X) * 2.f;
		ScreenPosition->X = -ViewportCenter.X - ScreenPosition->X;
	}

	PlayerController->ProjectWorldLocationToScreen(InLocation, *ScreenPosition);

	// Check to see if it's on screen. If it is, ProjectWorldLocationToScreen is all we need, return it.
	if (ScreenPosition->X >= 0.f && ScreenPosition->X <= ViewportSize.X
		&& ScreenPosition->Y >= 0.f && ScreenPosition->Y <= ViewportSize.Y)
	{
		OutScreenPosition = *ScreenPosition;
		bIsOnScreen = true;
		return;
	}

	*ScreenPosition -= ViewportCenter;

	float AngleRadians = FMath::Atan2(ScreenPosition->Y, ScreenPosition->X);
	AngleRadians -= FMath::DegreesToRadians(90.f);

	OutRotationAngleDegrees = FMath::RadiansToDegrees(AngleRadians) + 180.f;

	float Cos = cosf(AngleRadians);
	float Sin = -sinf(AngleRadians);

	ScreenPosition = new FVector2D(ViewportCenter.X + (Sin * 150.f), ViewportCenter.Y + Cos * 150.f);

	float m = Cos / Sin;

	FVector2D ScreenBounds = ViewportCenter * EdgePercent;

	if (Cos > 0)
	{
		ScreenPosition = new FVector2D(ScreenBounds.Y / m, ScreenBounds.Y);
	}
	else
	{
		ScreenPosition = new FVector2D(-ScreenBounds.Y / m, -ScreenBounds.Y);
	}

	if (ScreenPosition->X > ScreenBounds.X)
	{
		ScreenPosition = new FVector2D(ScreenBounds.X, ScreenBounds.X*m);
	}
	else if (ScreenPosition->X < -ScreenBounds.X)
	{
		ScreenPosition = new FVector2D(-ScreenBounds.X, -ScreenBounds.X*m);
	}

	*ScreenPosition += ViewportCenter;

	OutScreenPosition = *ScreenPosition;

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

	print(*resultPos.ToString());

	bIsOutScreen = isOutRange(resultPos, innerBorder);
	OutScreenPosition = CustomClamp(resultPos, innerBorder);

	if (bIsOutScreen) {
		OutRotationAngleDegrees = FMath::RadiansToDegrees(acosf(FVector2D::DotProduct(FVector2D(0,-1), OutScreenPosition.GetSafeNormal())));
	}
}

#pragma optimize("", on)