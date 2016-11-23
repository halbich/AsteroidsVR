

#pragma once

#include "Widgets/ObjectWidget.h"
#include "MainMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuCloseRequest);

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class ASTEROIDSVR_API UMainMenuWidget : public UObjectWidget
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(BlueprintAssignable, Category = "CustomWidgets|ObjectWidget")
		FMainMenuCloseRequest OnMainMenuCloseRequest;
	
};
