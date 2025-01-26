#include "BetterHUD.h"

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

void ABetterHUD::TogglePlayerUI(bool bShow)
{
	// TODO
}

void ABetterHUD::AddMenu(const TSubclassOf<class UCommonActivatableWidget> CommonWidget)
{
	if (!CommonWidget || !MenusContainerWidget || !IsValid(MenusContainerWidget))
		return;

	if (MenusContainerWidget->GetWidgetFromClass(CommonWidget))
	{
		UE_LOG(LogBetterUI, Warning, TEXT("Trying to show an existing widget, removing current one..."));
		RemoveMenu(CommonWidget);
	}

	MenusContainerWidget->AddWidgetFromClass(CommonWidget);
}

void ABetterHUD::RemoveMenu(const TSubclassOf<class UCommonActivatableWidget> CommonWidget)
{
	if (!CommonWidget || !MenusContainerWidget || !IsValid(MenusContainerWidget))
		return;

	UCommonActivatableWidget* CurrentWidget = MenusContainerWidget->GetWidgetFromClass(CommonWidget);

	if (!CurrentWidget)
		return;
	
	MenusContainerWidget->RemoveWidget(CurrentWidget);
	CurrentWidget = nullptr;
}
