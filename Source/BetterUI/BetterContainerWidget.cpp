#include "BetterContainerWidget.h"

#include "CommonUI/Public/Widgets/CommonActivatableWidgetContainer.h"
#include "BetterHUD.h"
#include "BetterMenuWidget.h"
#include "BetterUI.h"
#include "CommonButtonBase.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"

static UWidget* GetFocusedUMGWidget()
{
	const TSharedPtr<SWidget> FocusedSlateWidget = FSlateApplication::Get().GetUserFocusedWidget(0);
	if (!FocusedSlateWidget.IsValid())
	{
		return nullptr;
	}
	for (TObjectIterator<UWidget> Itr; Itr; ++Itr)
	{
		if (UWidget* CandidateUMGWidget = *Itr; CandidateUMGWidget->GetCachedWidget() == FocusedSlateWidget)
		{
			return CandidateUMGWidget;
		}
	}
	return nullptr;
}

UCommonActivatableWidget* UBetterContainerWidget::AddWidgetFromClass(
	TSubclassOf<class UCommonActivatableWidget> WidgetToAdd)
{
	if (!GetOwningPlayer()->IsLocalController())
		return nullptr;

	if (!WidgetStack || !WidgetToAdd)
		return nullptr;

	// TODO: Find a way to access the widget that still exists to avoid duplicate added widget.
	// GetWidgetList for some reason does not work, is EpicGames at fault?

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
	if (!this)
	{
		UE_LOG(LogBetterUI, Error, TEXT("Invalid 'this' pointer!"));
		return nullptr;
	}

	if (!WidgetToFind || !WidgetStack)
	{
		UE_LOG(LogBetterUI, Warning, TEXT("Invalid WidgetToFind or WidgetStack is null."));
		return nullptr;
	}

	const auto WidgetList = WidgetStack->GetWidgetList();
	if (WidgetList.Num() == 0)
	{
		UE_LOG(LogBetterUI, Warning, TEXT("WidgetStack has no widgets."));
		return nullptr;
	}

	for (const auto Element : WidgetList)
	{
		if (Element && Element->GetClass()->IsChildOf(WidgetToFind))
		{
			UE_LOG(LogBetterUI, Log, TEXT("Widget %s found"), *Element->GetName());
			return Element;
		}
	}

	UE_LOG(LogBetterUI, Warning, TEXT("Widget of class %s not found."), *WidgetToFind->GetName());
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
	if (!WidgetStack || WidgetStack->GetActiveWidget() == nullptr || !IsValid(WidgetStack->GetActiveWidget()) ||
		WidgetStack->GetNumWidgets() < 1)
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

	GetOwningLocalPlayer()->GetSubsystem<UCommonInputSubsystem>()->OnInputMethodChangedNative.AddWeakLambda(
		this, [this](ECommonInputType CommonInputType)
		{
			HandleFocus();
		});
}

void UBetterContainerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UWidget* CurrentFocusedWidget = GetFocusedUMGWidget();

	if (CurrentFocusedWidget != nullptr && CurrentFocusedWidget != LastFocusedWidget)
	{
		LastFocusedWidget = CurrentFocusedWidget;
		bPendingFocusReset = false;
	}

	if (CurrentFocusedWidget == nullptr && WidgetStack->GetNumWidgets() > 0)
	{
		if (!bPendingFocusReset)
		{
			bPendingFocusReset = true;
			LastFocusTime = GetWorld()->GetRealTimeSeconds();
		}

		if (GetWorld()->GetRealTimeSeconds() - LastFocusTime > 0.1f)
		{
			HandleFocus();
			bPendingFocusReset = false;
		}
	}
}


void UBetterContainerWidget::HandleFocus()
{
	if (!IsValid(WidgetStack->GetActiveWidget()) || WidgetStack->GetActiveWidget() == nullptr || WidgetStack->
		GetNumWidgets() < 1)
	{
		return;
	}
	
	if (!IsValid(LastFocusedWidget))
	{
		LastFocusedWidget = nullptr;
	}

	if (LastFocusedWidget != nullptr)
	{
		LastFocusedWidget->SetFocus();
		LastFocusedWidget->SetKeyboardFocus();
		return;
	}

	if (UWidget* FocusTarget = WidgetStack->GetActiveWidget()->GetDesiredFocusTarget())
	{
		FocusTarget->SetFocus();
		FocusTarget->SetKeyboardFocus();
	}
	else
	{
		UE_LOG(LogBetterUI, Error, TEXT("FocusTarget is null"));
	}
}
