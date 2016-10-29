// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "KinectSetupComponent.h"
#include "AsteroidsVRPawn.generated.h"

UCLASS(config = Game)
class AAsteroidsVRPawn : public APawn
{
	GENERATED_BODY()

		/** StaticMesh component that will be the visuals for our flying pawn */
		UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PlaneMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
public:
	AAsteroidsVRPawn();

	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides

protected:

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

public:
	UFUNCTION(BlueprintCallable, Category = "Plane Control")
		void LeftUp(float Val);

	UFUNCTION(BlueprintCallable, Category = "Plane Control")
		void LeftRight(float Val);

	UFUNCTION(BlueprintCallable, Category = "Plane Control")
		void RightUp(float Val);

	UFUNCTION(BlueprintCallable, Category = "Plane Control")
		void RightRight(float Val);


private:

	/** How quickly forward speed changes */
	UPROPERTY(Category = Plane, EditAnywhere)
		float ContinualAcceleration;

	/** How quickly breaks  */
	UPROPERTY(Category = Plane, EditAnywhere)
		float BrakeSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Plane, EditAnywhere)
		float MinSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Plane, EditAnywhere)
		float MaxSpeed;



	/** How quickly pawn can roll */
	UPROPERTY(Category = Rot, EditAnywhere)
		float RollSpeed;

	/** How quickly pawn can pitch */
	UPROPERTY(Category = Rot, EditAnywhere)
		float PitchSpeed;

	/** How quickly pawn can yaw */
	UPROPERTY(Category = Rot, EditAnywhere)
		float YawSpeed;



	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	bool UseKinect;

	UKinectSetupComponent* currentKinectConfig;

protected:

	UPROPERTY(Category = Plane, BlueprintReadOnly)
		FVector2D LeftControl;

	UPROPERTY(Category = Plane, BlueprintReadOnly)
		FVector2D RightControl;

public:
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	UFUNCTION(BlueprintCallable, Category = "Plane control")
		void SetNewUseKinect(bool useKinect);

	UFUNCTION(BlueprintCallable, Category = "Plane control")
		void RegisterSetupComponent(UKinectSetupComponent* kinectConfig);
};
