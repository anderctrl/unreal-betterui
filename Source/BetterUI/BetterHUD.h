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
	class UBetterContainerWidget* GetMenusWidget() const { return IsValid(MenusContainerWidget) ? MenusContainerWidget : nullptr; }

	UFUNCTION(BlueprintCallable)
	FKey GetKeyboardBackInput() const { return KeyboardBackInput; }

	UFUNCTION(BlueprintCallable)
	FKey GetGamepadBackInput() const { return GamepadBackInput; }

	UFUNCTION(BlueprintCallable)
	virtual UCommonActivatableWidget* AddWidget(TSubclassOf<class UCommonActivatableWidget> InCommonWidget, UBetterContainerWidget* ToContainer) const;

	UFUNCTION(BlueprintCallable)
	virtual void RemoveWidget(TSubclassOf<class UCommonActivatableWidget> InCommonWidget, UBetterContainerWidget* FromContainer) const;

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
