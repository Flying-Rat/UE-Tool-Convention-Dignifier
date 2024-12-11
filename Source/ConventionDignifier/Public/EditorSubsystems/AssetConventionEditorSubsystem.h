// Copyright 2022 Tadeáš Anděl, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ConventionSetup.h"
#include "EditorSubsystem.h"

#include "AssetConventionEditorSubsystem.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CONVENTIONDIGNIFIER_API UAssetConventionEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public: // Overrides
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public: // Asset Naming

	bool UpdateAssetName(const FAssetData& AssetData);

	UFUNCTION(BlueprintCallable)
	bool UpdateAssetName(UObject* Asset);

	TArray<FString> GetAssetMainName(const TArray<FString>& Names, const TArray<FString>& IntendedPrefixes, const TArray<FString>& IntendedSuffixes) const;

private: // SetupData

	UPROPERTY(Transient)
	TMap<FName, FConventionSetupData> ConventionSetupDataTable;
};
