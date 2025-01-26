#pragma once

#include "CoreMinimal.h"
#include "BetterContainerWidget.h"
#include "GameFramework/HUD.h"
#include "BetterHUD.generated.h"

UCLASS()
class BETTERUI_API ABetterHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABetterHUD();
	virtual void BeginPlay() override;
	virtual void ShowHUD() override;
	
	UFUNCTION(BlueprintCallable)
	void TogglePlayerUI(bool bShow);

	UFUNCTION(BlueprintCallable)
	class UBetterContainerWidget* GetMenusWidget() const { return IsValid(MenusContainerWidget) ? MenusContainerWidget : nullptr; }

	UFUNCTION(BlueprintCallable)
	FKey GetKeyboardBackInput() const { return KeyboardBackInput; }

	UFUNCTION(BlueprintCallable)
	FKey GetGamepadBackInput() const { return GamepadBackInput; }

	UFUNCTION(BlueprintCallable)
	void AddMenu(TSubclassOf<class UCommonActivatableWidget> CommonWidget);

	UFUNCTION(BlueprintCallable)
	void RemoveMenu(TSubclassOf<class UCommonActivatableWidget> CommonWidget);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UBetterContainerWidget> WidgetContainerClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	FKey KeyboardBackInput;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	FKey GamepadBackInput;
	
	UPROPERTY()
	class UBetterContainerWidget* MenusContainerWidget;
};
