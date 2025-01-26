#include "BetterHUD.h"

#include "BetterMenuWidget.h"
#include "BetterUI.h"

ABetterHUD::ABetterHUD()
{
	PrimaryActorTick.bCanEverTick = true;
	SetTickableWhenPaused(true);
}

void ABetterHUD::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetContainerClass)
	{
		MenusContainerWidget = CreateWidget<UBetterContainerWidget>(GetOwningPlayerController(), WidgetContainerClass);
		MenusContainerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MenusContainerWidget->AddToPlayerScreen(2);
	}
}

void ABetterHUD::ShowHUD()
{
	Super::ShowHUD();

	if (bShowHUD)
	{
		if (MenusContainerWidget) MenusContainerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		if (MenusContainerWidget) MenusContainerWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UCommonActivatableWidget* ABetterHUD::AddWidget(const TSubclassOf<class UCommonActivatableWidget> InCommonWidget,
	UBetterContainerWidget* ToContainer) const
{
	if (!InCommonWidget || !ToContainer || !IsValid(ToContainer))
		return nullptr;

	if (ToContainer->GetWidgetFromClass(InCommonWidget))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("Trying to show an existing widget on container %s, removing current one..."),
			*ToContainer->GetClass()->GetName());
		RemoveWidget(InCommonWidget, ToContainer);
	}

	return ToContainer->AddWidgetFromClass(InCommonWidget);
}

void ABetterHUD::RemoveWidget(const TSubclassOf<class UCommonActivatableWidget> InCommonWidget,
                              UBetterContainerWidget* FromContainer) const
{
	if (!InCommonWidget || !FromContainer || !IsValid(FromContainer))
		return;

	UCommonActivatableWidget* CurrentWidget = MenusContainerWidget->GetWidgetFromClass(InCommonWidget);

	if (!CurrentWidget)
		return;
	
	FromContainer->RemoveWidget(CurrentWidget);
	CurrentWidget = nullptr;
}
