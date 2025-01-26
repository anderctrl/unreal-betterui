// Copyright Epic Games, Inc. All Rights Reserved.

#include "BetterUI.h"

#define LOCTEXT_NAMESPACE "FBetterUIModule"

DEFINE_LOG_CATEGORY(LogBetterUI);

void FBetterUIModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FBetterUIModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBetterUIModule, BetterUI)