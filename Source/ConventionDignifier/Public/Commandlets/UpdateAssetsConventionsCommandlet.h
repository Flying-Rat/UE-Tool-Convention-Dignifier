// Copyright 2022 Tadeáš Anděl, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "UpdateAssetsConventionsCommandlet.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CONVENTIONDIGNIFIER_API UUpdateAssetsConventionsCommandlet : public UCommandlet
{
	GENERATED_BODY()

public: // Construction

	UUpdateAssetsConventionsCommandlet();

public: // Commandlet
	
	virtual int32 Main(const FString& FullCommandLine) override;
};
