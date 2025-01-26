
#include "BetterContainerWidget.h"

#include "CommonUI/Public/Widgets/CommonActivatableWidgetContainer.h"
#include "BetterHUD.h"
#include "BetterMenuWidget.h"
#include "BetterUI.h"

UCommonActivatableWidget* UBetterContainerWidget::AddWidgetFromClass(TSubclassOf<class UCommonActivatableWidget> WidgetToAdd)
{
	if (!GetOwningPlayer()->IsLocalController())
		return nullptr;

	if (!WidgetStack || !WidgetToAdd)
		return nullptr;

	for (UCommonActivatableWidget* Element : WidgetStack->GetWidgetList())
	{
		if (Element && Element->GetClass() == WidgetToAdd)
		{
			Element->ActivateWidget();
			return Element;
		}
	}

	UCommonActivatableWidget* NewWidget = WidgetStack->AddWidget(WidgetToAdd);
	if (NewWidget)
	{
		UE_LOG(LogBetterUI, Log, TEXT("Added %s widget"), *NewWidget->GetName());
	}
	return NewWidget;
}

UCommonActivatableWidget* UBetterContainerWidget::GetWidgetFromClass(
	TSubclassOf<class UCommonActivatableWidget> WidgetToFind) const
{
	for (const auto Element : WidgetStack->GetWidgetList())
	{
		if (Element->GetClass() == WidgetToFind)
		{
			return Element;
		}
	}
	return nullptr;
}

void UBetterContainerWidget::RemoveWidget(UCommonActivatableWidget* WidgetToRemove, bool bForceRemove)
{
	if (!GetOwningPlayer()->IsLocalController())
		return;

	if (!WidgetStack || !WidgetToRemove || !IsValid(WidgetToRemove))
	{
		return;
	}
	
	if (!bForceRemove && WidgetToRemove->IsA(UBetterMenuWidget::StaticClass()))
	{
		if (const UBetterMenuWidget* Menu = Cast<UBetterMenuWidget>(WidgetToRemove); Menu && !Menu->CanBeRemoved()) 
			return;
	}

	WidgetToRemove->DeactivateWidget();
	WidgetStack->RemoveWidget(*WidgetToRemove);
	UE_LOG(LogBetterUI, Log, TEXT("Removed %s widget"), *WidgetToRemove->GetName());
}

void UBetterContainerWidget::RemoveCurrentWidget()
{
	if (WidgetStack->GetNumWidgets() < 1)
		return;

	if (!IsValid(WidgetStack->GetActiveWidget()) || WidgetStack->GetActiveWidget() == nullptr)
		return;

	RemoveWidget(WidgetStack->GetActiveWidget());
}

bool UBetterContainerWidget::HasAnyWidgets()
{
	if (!IsValid(WidgetStack->GetActiveWidget()) || WidgetStack->GetActiveWidget() == nullptr || WidgetStack->
		GetNumWidgets() < 1)
	{
		return false;
	}

	return true;
}

int32 UBetterContainerWidget::GetNumberOfWidgets()
{
	return WidgetStack->GetNumWidgets();
}

void UBetterContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentHUD = Cast<ABetterHUD>(GetOwningPlayer()->GetHUD());

	WidgetStack->OnTransitioningChanged.AddWeakLambda(
		this, [this](const UCommonActivatableWidgetContainerBase* Widget, bool bIsTransitioning)
		{
			if (IsValid(Widget) && !bIsTransitioning)
			{
				HandleFocus();
			}
		});
}

void UBetterContainerWidget::HandleFocus()
{
	if (!IsValid(WidgetStack->GetActiveWidget()) || WidgetStack->GetActiveWidget() == nullptr || WidgetStack->
		GetNumWidgets() < 1)
	{
		return;
	}

	const UCommonActivatableWidget* ActiveWidget = WidgetStack->GetActiveWidget();

	if (UWidget* FocusTarget = ActiveWidget->GetDesiredFocusTarget())
	{

		FocusTarget->SetFocus();
		FocusTarget->SetKeyboardFocus();
	}
	else
	{
		UE_LOG(LogBetterUI, Error, TEXT("FocusTarget is null"));
	}
}
