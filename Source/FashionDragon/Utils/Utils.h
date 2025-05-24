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
};
