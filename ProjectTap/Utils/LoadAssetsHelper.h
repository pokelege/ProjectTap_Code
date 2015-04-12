#pragma once
#include "ProjectTap.h"

static FORCEINLINE FName GetObjPath(const UObject* obj)
{
	FName name = NAME_None;

	if (obj != nullptr && obj->IsValidLowLevel())
	{
		auto path = FStringAssetReference(obj);

		if (path.IsValid())
		{
			auto path_str = obj->GetClass()->GetDescription();
			path_str += "'";
			path_str += path.ToString();
			path_str += "'";

			name = FName(*path_str);
		}
	}


	return name;
}
 
template <class AssetsClass>
static FORCEINLINE AssetsClass* LoadAssetFromPath(const FName& path)
{
	AssetsClass* result = nullptr;

	if (path != NAME_None)
	{
		result = Cast<AssetsClass>(StaticLoadObject(AssetsClass::StaticClass(), nullptr, *path.ToString()));
	}

	return result;
}