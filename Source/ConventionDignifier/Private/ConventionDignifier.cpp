// Copyright 2022 Tadeáš Anděl, All Rights Reserved.

#include "ConventionDignifier.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"

#include "AssetActions/AssetConventionAction.h"

#include "AssetRegistry/AssetRegistryModule.h"

#include "EditorSubsystems/AssetConventionEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "FConventionDignifierModule"

void FConventionDignifierModule::StartupModule()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	AssetRegistryModule.Get().OnAssetAdded().AddRaw(this, &FConventionDignifierModule::HandleAssetAdded);
}

void FConventionDignifierModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
		for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}
	CreatedAssetTypeActions.Empty();
}

void FConventionDignifierModule::HandleAssetAdded(const FAssetData& AssetData)
{
	if (!IsValid(GWorld))
	{
		return;
	}
	
	if (AssetData.PackagePath.ToString().Contains("Engine"))
	{
		return;
	}

	if (!AssetData.HasAnyPackageFlags(PKG_NewlyCreated))
	{
		return;
	}

	// Factory does not like immediate renaming of new assets and crashes, so schedule it for next tick
	GWorld->GetTimerManager().SetTimerForNextTick([AssetData]()
	{
		UAssetConventionEditorSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UAssetConventionEditorSubsystem>();
		EditorAssetSubsystem->UpdateAssetName(AssetData);
	});
}

void FConventionDignifierModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FConventionDignifierModule, ConventionDignifier)