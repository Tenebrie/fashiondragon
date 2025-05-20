#pragma once

struct FPoseWingEffector
{
	float Flap = 0.0f;
	float Openness = 0.0f;

	FPoseWingEffector() {}
	FPoseWingEffector(FPoseWingEffector const&) = default;
	FPoseWingEffector(const float Flap, const float Openness): Flap(Flap), Openness(Openness) {}

	FPoseWingEffector AddFlap(const float Delta) const
	{
		return FPoseWingEffector(Flap + Delta, Openness);
	}
	FPoseWingEffector AddOpenness(const float Delta) const
	{
		return FPoseWingEffector(Flap, Openness + Delta);
	}
};
