#pragma once
#include "Curves/CurveEvaluation.h"

struct FArticulatedValue
{
	float Value = 0.0f;
	float StartArticulation = 0.0f;
	float EndArticulation = 0.0f;

	FArticulatedValue() = default;
	explicit FArticulatedValue(const float Value): Value(Value) {}
	FArticulatedValue(const float Value, const float StartArticulation, const float EndArticulation)
		: Value(Value), StartArticulation(StartArticulation), EndArticulation(EndArticulation) {}

	float CalculateAt(const float StartingValue, const float Alpha) const
	{
		return UE::Curves::BezierInterp(
			StartingValue,
			StartingValue + StartArticulation,
			Value + EndArticulation,
			Value,
			Alpha
		);
	}
};
