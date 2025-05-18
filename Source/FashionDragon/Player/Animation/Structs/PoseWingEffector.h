#pragma once

struct FPoseWingEffector
{
	float Flap = 0.0f;
	float Openness = 0.0f;

	FPoseWingEffector() {}
	FPoseWingEffector(FPoseWingEffector const&) = default;
	FPoseWingEffector(const float Flap, const float Openness): Flap(Flap), Openness(Openness) {}
};
