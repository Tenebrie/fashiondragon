#pragma once

class FUtils
{
public:
	static float GetRotatorDistance(const FRotator& A);
	static float GetRotatorDistance(const FRotator&, const FRotator&);
	template <typename T, typename... Ts>
	static constexpr bool IsOneOf(T Target, Ts... Options) noexcept {
		return ((Target == Options) || ...);
	}
	template<typename EnumType>
	static FString EnumToString(EnumType Value);
};

template<typename EnumType>
FString FUtils::EnumToString(EnumType Value)
{
	const UEnum* EnumPtr = StaticEnum<EnumType>();
	if (!EnumPtr)
	{
		return TEXT("Invalid");
	}
	return EnumPtr->GetNameStringByValue(static_cast<int64>(Value));
}
