

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Materials/MaterialInterface.h"
#include "Misc/Guid.h"
#include "MaterialTypes.h"
#include "UCharacterReplacerUtils.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERREPLACER_API UUCharacterReplacerUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "CRUtils")
	static void GetAllMaterialParameterNames(UMaterialInterface* material, UPARAM(ref) TArray<FName>& scalarNamesOut, UPARAM(ref) TArray<FName>& vectorNamesOut)
	{
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
};
