// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterReplacer.h"
#include "CharacterReplacerDataSubsystem.h"
#include "Patching/NativeHookManager.h"
#include "FGCharacterPlayer.h"
#include "Subsystem/ModSubsystem.h"
#include "Subsystem/SubsystemActorManager.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "FCharacterReplacerModule"

void FCharacterReplacerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (!WITH_EDITOR)
	{
		SUBSCRIBE_METHOD_AFTER(AFGCharacterPlayer::UpdateEquipmentVisibility, [](AFGCharacterPlayer* self) {
			if (self == nullptr) return;
			UWorld* worldObject = GEngine->GetWorldFromContextObjectChecked(self);
			if (worldObject == nullptr) return;
			USubsystemActorManager* subsysManager = worldObject->GetSubsystem<USubsystemActorManager>();
			if (subsysManager == nullptr) return;
			check(subsysManager);
			ACharacterReplacerDataSubsystem* dataSubsys = subsysManager->GetSubsystemActor<ACharacterReplacerDataSubsystem>();
			if (dataSubsys == nullptr) return;
			dataSubsys->OnEquipmentVisibilityUpdated(self);
			});
	}
}

void FCharacterReplacerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCharacterReplacerModule, CharacterReplacer)