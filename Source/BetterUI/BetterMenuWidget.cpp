#include "BetterMenuWidget.h"

#include "BetterHUD.h"
#include "BetterUI.h"

void UBetterMenuWidget::SetCanBeRemoved(bool bNewCanBeRemoved)
{
	bCanBeRemoved = bNewCanBeRemoved;
}

void UBetterMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeHUD();
}

void UBetterMenuWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}

FReply UBetterMenuWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (!CurrentHUD->IsA(ABetterHUD::StaticClass()))
		return FReply::Unhandled();

	UE_LOG(LogBetterUI, Warning, TEXT("UBetterMenuWidget::NativeOnPreviewKeyDown"));
	
	if (CanBeRemoved() && (InKeyEvent.GetKey() == CurrentHUD->GetGamepadBackInput() || InKeyEvent.GetKey() == CurrentHUD->GetKeyboardBackInput()))
	{
		RemoveSelfFromContainer();
	}
	
	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UBetterMenuWidget::InitializeHUD()
{
	if (const APlayerController* PlayerController = GetOwningPlayer())
	{
		CurrentHUD = Cast<ABetterHUD>(PlayerController->GetHUD());
	}
}

void UBetterMenuWidget::RemoveSelfFromContainer()
{
	OnWantsToCloseMenu.Broadcast();
	CurrentHUD->GetMenusWidget()->RemoveWidget(this);
}
