// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AsteroidsVR.h"
#include "AsteroidsVRGameMode.h"
#include "AsteroidsVRPawn.h"

AAsteroidsVRGameMode::AAsteroidsVRGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AAsteroidsVRPawn::StaticClass();

}
