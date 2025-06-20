

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Materials/MaterialInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/MorphTarget.h"
#include "Misc/Guid.h"
#include "MaterialTypes.h"
#include "CharacterReplacerUtils.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERREPLACER_API UCharacterReplacerUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "CRUtils")
	static void GetAllMaterialParameterNames(UMaterialInterface* material, UPARAM(ref) TArray<FName>& scalarNamesOut, UPARAM(ref) TArray<FName>& vectorNamesOut)
	{
		if (material == nullptr) return;
		TArray < FMaterialParameterInfo > OutParameterInfo;
		TArray < FGuid > OutParameterIds;
		material->GetAllScalarParameterInfo(OutParameterInfo, OutParameterIds);
		for (FMaterialParameterInfo info : OutParameterInfo)
		{
			scalarNamesOut.AddUnique(info.Name);
		}
		OutParameterInfo.Empty();
		material->GetAllVectorParameterInfo(OutParameterInfo, OutParameterIds);
		for (FMaterialParameterInfo info : OutParameterInfo)
		{
			vectorNamesOut.AddUnique(info.Name);
		}
	}

	UFUNCTION(BlueprintCallable, Category = "CRUtils")
	static void GetAllMaterialParameterValues(UMaterialInterface* material, UPARAM(ref) TMap<FName, float>& scalarsRef, UPARAM(ref) TMap<FName, FLinearColor>& vectorsRef)
	{
		if (material == nullptr) return;
		TArray < FMaterialParameterInfo > OutParameterInfo;
		TArray < FGuid > OutParameterIds;
		material->GetAllScalarParameterInfo(OutParameterInfo, OutParameterIds);
		for (FMaterialParameterInfo info : OutParameterInfo)
		{
			if (!scalarsRef.Contains(info.Name))
			{
				float value = 0;
				material->GetScalarParameterValue(info, value, false);
				scalarsRef.Add(info.Name, value);
			}
		}
		OutParameterInfo.Empty();
		material->GetAllVectorParameterInfo(OutParameterInfo, OutParameterIds);
		for (FMaterialParameterInfo info : OutParameterInfo)
		{
			if (!vectorsRef.Contains(info.Name))
			{
				FLinearColor value;
				material->GetVectorParameterValue(info, value, false);
				vectorsRef.Add(info.Name, value);
			}
		}
	}

	UFUNCTION(BlueprintCallable, Category = "CRUtils")
	static bool IsParameterizedMaterial(UMaterialInterface* material)
	{
		if (material == nullptr) return false;
		TArray < FMaterialParameterInfo > OutParameterInfo;
		TArray < FGuid > OutParameterIds;
		material->GetAllScalarParameterInfo(OutParameterInfo, OutParameterIds);
		if (OutParameterInfo.Num() > 0)
		{
			return true;
		}
		OutParameterInfo.Empty();
		material->GetAllVectorParameterInfo(OutParameterInfo, OutParameterIds);
		return OutParameterInfo.Num() > 0;
	}

	UFUNCTION(BlueprintCallable, Category = "CRUtils")
	static void GetMorphTargetOverrides(USkeletalMeshComponent* meshComponent, UPARAM(ref) TMap<FName, float>& morphTargetsRef)
	{
		if (meshComponent == nullptr) return;
		morphTargetsRef.Append(meshComponent->GetMorphTargetCurves());
	}

	UFUNCTION(BlueprintCallable, Category = "CRUtils")
	static void GetAllMorphTargetNames(USkeletalMesh* mesh, UPARAM(ref) TArray<FName>& morphTargetNamesRef)
	{
		if (mesh == nullptr) return;
		for (UMorphTarget* morphTarget : mesh->GetMorphTargets())
		{
			morphTargetNamesRef.AddUnique(morphTarget->GetFName());
		}
	}
};
