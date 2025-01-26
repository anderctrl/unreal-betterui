#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "BetterMenuWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWantsToCloseMenu);

UCLASS()
class BETTERUI_API UBetterMenuWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	FOnWantsToCloseMenu OnWantsToCloseMenu;

	UFUNCTION(BlueprintCallable)
	void SetCanBeRemoved(bool bNewCanBeRemoved);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CanBeRemoved() const { return bCanBeRemoved; };

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;

	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void InitializeHUD();

	UPROPERTY()
	bool bCanBeRemoved { true };

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	class ABetterHUD* CurrentHUD;
	
	UFUNCTION()
	virtual void RemoveSelfFromContainer();
};

