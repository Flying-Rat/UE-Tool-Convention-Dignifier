// Copyright 2022 Tadeáš Anděl, All Rights Reserved.

#include "ConventionSetup.h"

#include "Misc/DataValidation.h"

#if WITH_EDITOR
EDataValidationResult UConventionSetup::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	for (const TTuple<TSubclassOf<UObject>, FConventionSetupData>& Setup : ConventionSetup)
	{
		if (!IsValid(Setup.Key))
		{
			Context.AddError(FText::FromString(TEXT("Invalid class in convention setup.")));
		}
	}

	return Context.GetNumErrors() > 0 ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif