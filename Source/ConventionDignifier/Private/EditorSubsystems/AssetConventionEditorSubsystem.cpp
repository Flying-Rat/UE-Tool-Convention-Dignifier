// Copyright 2022 Tadeáš Anděl, All Rights Reserved.


#include "EditorSubsystems/AssetConventionEditorSubsystem.h"

#include "Subsystems/EditorAssetSubsystem.h"

void UAssetConventionEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	static const FString ConventionSetupReference = "/Script/ConventionDignifier.ConventionSetup'/ConventionDignifier/DA_ConventionSetup.DA_ConventionSetup'";
	TSoftObjectPtr<UConventionSetup> ConventionSetupPath(ConventionSetupReference);
	UConventionSetup* ConventionSetup = ConventionSetupPath.LoadSynchronous();
	if (!IsValid(ConventionSetup))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find convention setup on reference path %s"), *ConventionSetupReference);
		return;
	}
	
	for (const TTuple<TSubclassOf<UObject>, FConventionSetupData>& Setup : ConventionSetup->GetConventionSetup())
	{
		if (!IsValid(Setup.Key))
		{
			UE_LOG(LogTemp, Error, TEXT("Unknown class in convention setup"));
			continue;
		}
		
		FName Key = Setup.Key->GetFName();
		ConventionSetupDataTable.Add(Key, Setup.Value);
	}

	for (const TTuple<FString, FConventionSetupData>& Setup : ConventionSetup->GetFallbackConventionSetup())
	{
		FName Key(Setup.Key);
		if (ConventionSetupDataTable.Contains(Key))
		{
			continue;
		}
		
		ConventionSetupDataTable.Add(Key, Setup.Value);
	}
}

bool UAssetConventionEditorSubsystem::UpdateAssetName(const FAssetData& AssetData)
{
	UObject* Asset = AssetData.GetAsset();
	if (!IsValid(Asset))
	{
		return false;
	}

	return UpdateAssetName(Asset);
}

bool UAssetConventionEditorSubsystem::UpdateAssetName(UObject* Asset)
{
	UEditorAssetSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!IsValid(EditorAssetSubsystem))
	{
		return false;
	}
	
	FConventionSetupData* AssetConventionSetup = nullptr;
	UClass* AssetClass = Asset->GetClass();
	if (AssetClass->IsChildOf(UBlueprint::StaticClass()))
	{
		UBlueprint* BlueprintClass = Cast<UBlueprint>(Asset);
		if (IsValid(BlueprintClass->GeneratedClass))
		{
			AssetClass = BlueprintClass->ParentClass;
		}
	}
	
	while (IsValid(AssetClass))
	{
		FName AssetClassName = AssetClass->GetFName();
		if (ConventionSetupDataTable.Contains(AssetClassName))
		{
			AssetConventionSetup = &ConventionSetupDataTable[AssetClassName];
			break;
		}
			
		AssetClass = AssetClass->GetSuperClass();
	}
	
	if (!AssetConventionSetup)
	{
		return false;
	}

	FString AssetName = Asset->GetName();
	TArray<FString> AssetSeparators;
	static const FString Separator = TEXT("_");
	AssetName.ParseIntoArray(AssetSeparators, *Separator, true);

	TArray<FString> AssetMainNameSeparators = GetAssetMainName(AssetSeparators, AssetConventionSetup->Prefixes, AssetConventionSetup->Suffixes);
	TArray<FString> NewAssetNameSeparators;
	for (FString IntendedPrefix : AssetConventionSetup->Prefixes)
	{
		NewAssetNameSeparators.Add(IntendedPrefix);
	}

	for (FString AssetSeparator : AssetMainNameSeparators)
	{
		NewAssetNameSeparators.Add(AssetSeparator);
	}

	for (FString IntendedSuffix : AssetConventionSetup->Suffixes)
	{
		NewAssetNameSeparators.Add(IntendedSuffix);
	}
	const FString& AssetFullPath = Asset->GetPathName();
	const FString& AssetDir = FPaths::GetPath(AssetFullPath);
	const FString& NewAssetName = FString::Join(NewAssetNameSeparators, *Separator);
	const FString& NewAssetFullPath = FPaths::Combine(AssetDir, NewAssetName + "." + NewAssetName);

	if (NewAssetFullPath == AssetFullPath)
	{
		return false;
	}
		
	if (EditorAssetSubsystem->RenameAsset(AssetFullPath, NewAssetFullPath))
	{
		UE_LOG(LogTemp, Log, TEXT("Renamed asset %s to %s"), *AssetName, *NewAssetName);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to rename asset %s to %s"), *AssetName, *NewAssetName);
	}
	return true;
}

TArray<FString> UAssetConventionEditorSubsystem::GetAssetMainName(const TArray<FString>& Names, const TArray<FString>& IntendedPrefixes, const TArray<FString>& IntendedSuffixes) const
{
	TArray<FString> Ret;
	int FirstIndex = 0;
	for (int i = 0; i < IntendedPrefixes.Num(); ++i)
	{
		if (!Names.IsValidIndex(i))
		{
			break;
		}

		if (Names[i] == IntendedPrefixes[i])
		{
			FirstIndex++;
		}
		else
		{
			break;
		}
	}

	int LastIndex = FMath::Max(Names.Num() - IntendedSuffixes.Num(), FirstIndex);

	for (int i = FirstIndex; i < LastIndex; ++i)
	{
		Ret.Add(Names[i]);
	}
	
	return Ret;
}
