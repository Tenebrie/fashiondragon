#pragma once
#include <typeinfo>

class FObject
{
public:
	virtual FString GetClassName()
	{
		const auto Name = FString(typeid(*this).name());
		return Name.RightChop(Name.Find(TEXT(" ")) + 1);
	}
	virtual ~FObject() = default;
};
