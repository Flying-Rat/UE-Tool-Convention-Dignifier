// Copyright 2022 Tadeáš Anděl, All Rights Reserved.

#pragma once

#include "AssetTypeActions_Base.h"

#include "EditorSubsystems/AssetConventionEditorSubsystem.h"

#define CREATE_ASSET_CONVENTION_ACTION(UserClass, Color) \
class FAssetTypeActions_##UserClass : public FAssetTypeActions_Base \
{ \
public: \
    FORCEINLINE virtual FText GetName() const override \
    { \
        return FText::FromString(""); \
    } \
    \
    FORCEINLINE virtual UClass* GetSupportedClass() const override \
    { \
        return UserClass::StaticClass(); \
    } \
    \
    FORCEINLINE virtual uint32 GetCategories() override \
    { \
        return EAssetTypeCategories::Basic; \
    } \
    \
    FORCEINLINE virtual bool CanFilter() override \
    { \
        return true; \
    } \
    \
    FORCEINLINE virtual FColor GetTypeColor() const override \
    { \
        return Color; \
    } \
    \
    FORCEINLINE virtual bool HasActions(const TArray<UObject*>& InObjects) const override \
    { \
        return true; \
    } \
    \
    FORCEINLINE virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override \
    { \
        FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder); \
        \
        TArray<TWeakObjectPtr<UObject>> Assets = GetTypedWeakObjectPtrs<UObject>(InObjects); \
        \
        FText ButtonLabel = FText::FromString("Update Conventions"); \
        FText ButtonTooltip = FText::FromString("Update conventions for selected assets from ConventionSetup asset"); \
        \
        TDelegate<TDelegate<void()>::RetValType()> ExecuteAction = FExecuteAction::CreateSP(this, &FAssetTypeActions_##UserClass::ExecuteUpdateConvention, Assets); \
        \
        MenuBuilder.AddMenuEntry(ButtonLabel, ButtonTooltip, FSlateIcon(), ExecuteAction); \
    } \
    \
    FORCEINLINE void ExecuteUpdateConvention(TArray<TWeakObjectPtr<UObject>> Objects) \
    { \
        UAssetConventionEditorSubsystem* AssetConventionEditorSubsystem = GEditor->GetEditorSubsystem<UAssetConventionEditorSubsystem>(); \
        for (TWeakObjectPtr<UObject> Object : Objects) \
        { \
            if (!Object.IsValid()) \
            { \
                continue; \
            } \
            \
            AssetConventionEditorSubsystem->UpdateAssetName(Object.Get()); \
        } \
    } \
};

CREATE_ASSET_CONVENTION_ACTION(UTexture2D, FColor(192, 64, 64))
CREATE_ASSET_CONVENTION_ACTION(UBlueprint, FColor(63, 126, 255))