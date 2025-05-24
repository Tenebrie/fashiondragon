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
	FPoseWingEffector SetFlap(const float NewFlap) const
	{
		return FPoseWingEffector(NewFlap, Openness);
	}
	FPoseWingEffector SetOpenness(const float NewOpenness) const
	{
		return FPoseWingEffector(Flap, NewOpenness);
	}
};
