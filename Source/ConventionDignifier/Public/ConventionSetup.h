// Copyright 2022 Tadeáš Anděl, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ConventionSetup.generated.h"

USTRUCT(BlueprintType)
struct FConventionSetupData
{
	GENERATED_BODY()

public: // Setup
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Convention")
	TArray<FString> Prefixes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Convention")
	TArray<FString> Suffixes;
};

UCLASS(BlueprintType, Blueprintable)
class CONVENTIONDIGNIFIER_API UConventionSetup : public UDataAsset
{
	GENERATED_BODY()

public: // Editor

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

public: // Data

	FORCEINLINE const TMap<TSubclassOf<UObject>, FConventionSetupData>& GetConventionSetup() const
	{
		return ConventionSetup;
	}

	FORCEINLINE const TMap<FString, FConventionSetupData>& GetFallbackConventionSetup() const
	{
		return FallbackConventionSetup;
	}

protected: // Setup

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Convention")
	TMap<TSubclassOf<UObject>, FConventionSetupData> ConventionSetup;
	
	/**
	 * When an asset is not found within ConventionSetup you can still specify it's name here
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Convention")
	TMap<FString, FConventionSetupData> FallbackConventionSetup;
};