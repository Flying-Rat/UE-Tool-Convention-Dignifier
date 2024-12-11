// Copyright 2022 Tadeáš Anděl, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IAssetTools;
class IAssetTypeActions;

class FConventionDignifierModule : public IModuleInterface
{
public: // Module
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private: // Callbacks
	
	void HandleAssetAdded(const FAssetData& AssetData);

private: // Asset Actions

	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);
	
private: // Asset Actions

	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};
