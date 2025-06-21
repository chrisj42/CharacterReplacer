
#pragma once

#include "CoreMinimal.h"
#include "FGCharacterPlayer.h"
#include "Subsystem/ModSubsystem.h"
#include "Subsystem/SubsystemActorManager.h"
#include "CharacterReplacer.h"
#include "CharacterReplacerDataSubsystem.generated.h"

UCLASS(Blueprintable)
class CHARACTERREPLACER_API ACharacterReplacerDataSubsystem : public AModSubsystem
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	//ACharacterReplacerDataSubsystem();
	//virtual ~ACharacterReplacerDataSubsystem();

	UFUNCTION(BlueprintImplementableEvent, Category = "CRUtils")
	void OnEquipmentVisibilityUpdated(AFGCharacterPlayer* player);
};
