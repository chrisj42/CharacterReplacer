
#include "CharacterReplacerDataSubsystem.h"
#include "Patching/NativeHookManager.h"

void ACharacterReplacerDataSubsystem::BeginPlay()
{
	Super::BeginPlay();
	if (!WITH_EDITOR)
	{
		SUBSCRIBE_METHOD_AFTER(AFGCharacterPlayer::UpdateEquipmentVisibility, [this](AFGCharacterPlayer* self) {
			if (self == nullptr || this == nullptr) return;
			this->OnEquipmentVisibilityUpdated(self);
			});
	}
}
