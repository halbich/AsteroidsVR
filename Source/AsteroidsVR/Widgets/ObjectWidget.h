

#pragma once

#include "Blueprint/UserWidget.h"
#include "Widgets/SynchronizeWidget.h"
#include "ObjectWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetCloseRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetShown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetRemoved, UObjectWidget*, RemovedWidget);

/**
 *
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class ASTEROIDSVR_API UObjectWidget : public USynchronizeWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "CustomWidgets|ObjectWidget")
		FWidgetCloseRequest OnWidgetCloseRequest;

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void RequestClose();

	UPROPERTY(BlueprintAssignable, Category = "CustomWidgets|ObjectWidget")
		FWidgetShown OnWidgetShown;

	UPROPERTY(BlueprintAssignable, Category = "CustomWidgets|ObjectWidget")
		FWidgetRemoved OnWidgetRemoved;

	virtual void OnEnterKey();

	// Returns if caller should be hidden from screen
	virtual bool OnEscapeKey();

	void WidgetShown();

	UPROPERTY()
		TArray<UObjectWidget*> ItemsStack;

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void AddToStack(UObjectWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void SwapWithTop(UObjectWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void AddToStackAndScreen(UObjectWidget* widget, int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "CustomWidgets|ObjectWidget")
		void RemoveTopWidgetFromStack();




private:


	FORCEINLINE void notifyWidgetRemoved(UObjectWidget* widget)
	{
		OnWidgetRemoved.Broadcast(widget);
	}

	FORCEINLINE void popAll() {
		while (ItemsStack.Num() > 0)
			pop();
	}

	void pop()
	{
		auto widget = ItemsStack.Pop();
		widget->popAll();
		notifyWidgetRemoved(widget);
		widget->notifyWidgetRemoved(widget);
	}

};
