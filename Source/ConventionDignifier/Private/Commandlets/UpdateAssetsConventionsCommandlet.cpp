// Copyright 2022 Tadeáš Anděl, All Rights Reserved.


#include "Commandlets/UpdateAssetsConventionsCommandlet.h"

#include "AssetRegistry/AssetRegistryModule.h"

#include "EditorSubsystems/AssetConventionEditorSubsystem.h"

class UAssetConventionEditorSubsystem;

UUpdateAssetsConventionsCommandlet::UUpdateAssetsConventionsCommandlet()
{
	FastExit = true;
	LogToConsole = true;
}

int32 UUpdateAssetsConventionsCommandlet::Main(const FString& FullCommandLine)
{
	UE_LOG(LogInit, Log, TEXT("Updating assets conventions"));
	
	TArray<FString> Tokens;
	TArray<FString> Switches;
	TMap<FString, FString> Params;
	ParseCommandLine(*FullCommandLine, Tokens, Switches, Params);

	// Initialize asset registry
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Get all assets in the project
	TArray<FAssetData> AllAssets;
	AssetRegistry.SearchAllAssets(true);
	if (Params.Contains("AssetsDirectories"))
	{
		FString AssetsDirectoriesString(Params["AssetsDirectories"]);
		TArray<FString> AssetsDirectories;
		AssetsDirectoriesString.ParseIntoArray(AssetsDirectories, TEXT(","));
		TArray<FName> AssetsFNameDirectories;
		for (const FString& AssetsDirectory : AssetsDirectories)
		{
			AssetsFNameDirectories.Add(FName(AssetsDirectory));
		}
		AssetRegistry.GetAssetsByPaths(AssetsFNameDirectories, AllAssets, true);
	}
	else
	{
		AssetRegistry.GetAllAssets(AllAssets);
	}

	if (!Switches.Contains(TEXT("bIncludeEngine")))
	{
		FString EngineDir = FPaths::ConvertRelativePathToFull(FPaths::EngineDir());
		AllAssets.RemoveAll([&EngineDir](const FAssetData& AssetData)
			{
				// Remove /Engine and any plugins from /Engine, but keep /Game and any plugins under /Game.
				FString FileName;
				FString PackageName;
				AssetData.PackageName.ToString(PackageName);
				if (!FPackageName::TryConvertLongPackageNameToFilename(PackageName, FileName))
				{
					// We don't recognize this packagepath, so keep it
					return false;
				}
				// ConvertLongPackageNameToFilename can return ../../Plugins for some plugins instead of
				// ../../../Engine/Plugins. We should fix that in FPackageName to always return the normalized
				// filename. For now, workaround it by converting to absolute paths.
				FileName = FPaths::ConvertRelativePathToFull(MoveTemp(FileName));
				return FPathViews::IsParentPathOf(EngineDir, FileName);
			});
	}
	
	UAssetConventionEditorSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UAssetConventionEditorSubsystem>();
	for (const FAssetData& AssetData : AllAssets)
	{
		EditorAssetSubsystem->UpdateAssetName(AssetData);
	}
	
	return 0;
}