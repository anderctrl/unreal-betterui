#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UObject/Object.h"
#include "BetterContainerWidget.generated.h"

UCLASS()
class BETTERUI_API UBetterContainerWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	UCommonActivatableWidget* AddWidgetFromClass(TSubclassOf<class UCommonActivatableWidget> WidgetToAdd);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UCommonActivatableWidget* GetWidgetFromClass(TSubclassOf<class UCommonActivatableWidget> WidgetToFind) const;
	
	UFUNCTION(BlueprintCallable)
	void RemoveWidget(class UCommonActivatableWidget* WidgetToRemove, bool bForceRemove = false);

	UFUNCTION(BlueprintCallable)
	void RemoveCurrentWidget();

	UFUNCTION(BlueprintCallable)
	bool HasAnyWidgets();

	UFUNCTION(BlueprintCallable)
	int32 GetNumberOfWidgets();

	UFUNCTION(BlueprintCallable)
	class ABetterHUD* GetCurrentHUD() const { return CurrentHUD; }

	UFUNCTION(BlueprintCallable)
	void HandleFocus();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess=true))
	class UCommonActivatableWidgetStack* WidgetStack;

	UPROPERTY()
	class ABetterHUD* CurrentHUD;

	UPROPERTY()
	UWidget* LastFocusedWidget;

private:
	bool bPendingFocusReset = false;
	float LastFocusTime = 0.0f;
};
