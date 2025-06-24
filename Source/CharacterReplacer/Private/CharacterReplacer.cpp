// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterReplacer.h"
#include "CharacterReplacerDataSubsystem.h"
#include "Patching/NativeHookManager.h"
#include "FGCharacterPlayer.h"
#include "Subsystem/ModSubsystem.h"
#include "Subsystem/SubsystemActorManager.h"
#include "Engine/Engine.h"
#include "UObject/UObjectGlobals.h"

#define LOCTEXT_NAMESPACE "FCharacterReplacerModule"

DEFINE_LOG_CATEGORY(LogCharacterReplacer);

void FCharacterReplacerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (!WITH_EDITOR)
	{
		SUBSCRIBE_METHOD_AFTER(AFGCharacterPlayer::UpdateEquipmentVisibility, [](AFGCharacterPlayer* self) {
			//UE_LOG(LogCharacterReplacer, Verbose, TEXT("Checking player pointer"));
			if (self == nullptr) return;
			UWorld* worldObject = self->GetWorld();
			//UE_LOG(LogCharacterReplacer, Verbose, TEXT("Checking world pointer"));
			if (worldObject == nullptr) return;
			USubsystemActorManager* subsysManager = worldObject->GetSubsystem<USubsystemActorManager>();
			//UE_LOG(LogCharacterReplacer, Verbose, TEXT("Checking subsys manager pointer"));
			if (subsysManager == nullptr) return;
			check(subsysManager);
			UClass* SubsysBPClassRaw = StaticLoadClass(ACharacterReplacerDataSubsystem::StaticClass(), nullptr, TEXT("/CharacterReplacer/Logic/Subsys_Data_CR.Subsys_Data_CR_C"));
			if (SubsysBPClassRaw == nullptr) {
				//UE_LOG(LogCharacterReplacer, Verbose, TEXT("subsys class could not be found."));
				return;
			}
			TSubclassOf<ACharacterReplacerDataSubsystem> SubsysBPClass = SubsysBPClassRaw;
			AModSubsystem* dataSubsysParent = subsysManager->K2_GetSubsystemActor(SubsysBPClass);
			//UE_LOG(LogCharacterReplacer, Verbose, TEXT("Checking subsys pointer"));
			if (dataSubsysParent == nullptr) return;
			ACharacterReplacerDataSubsystem* dataSubsys = Cast<ACharacterReplacerDataSubsystem>(dataSubsysParent);
			//UE_LOG(LogCharacterReplacer, Verbose, TEXT("all checks passed, calling hook"));
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