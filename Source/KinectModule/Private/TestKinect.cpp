// Fill out your copyright notice in the Description page of Project Settings.

#include "KinectModule.h"
#include "TestKinect.h"


// Sets default values
ATestKinect::ATestKinect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestKinect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestKinect::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

