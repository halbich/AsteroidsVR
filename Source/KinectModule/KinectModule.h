
#pragma once

#include "Engine.h"
//#include "KinectModuleClasses.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::White,text)

DECLARE_LOG_CATEGORY_EXTERN(LogKinectModule, Log, All);

