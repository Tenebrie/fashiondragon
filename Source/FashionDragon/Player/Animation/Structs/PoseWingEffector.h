#pragma once
#include "PoseWingEffector.generated.h"

USTRUCT(BlueprintType)
struct FPoseWingEffector
{
	GENERATED_BODY()
	
	UPROPERTY() float Openness = 0.0f;
	UPROPERTY() float FlapAngle = 0.0f;
	UPROPERTY() float TiltAngle = 0.0f;

	FPoseWingEffector() {}
	FPoseWingEffector(FPoseWingEffector const&) = default;
	FPoseWingEffector(const float Openness, const float FlapAngle, const float TiltAngle): Openness(Openness), FlapAngle(FlapAngle), TiltAngle(TiltAngle) {}

	FPoseWingEffector AddOpenness(const float Delta) const
	{
		FPoseWingEffector Copy = *this;
		Copy.Openness += Delta;
		return Copy;
	}
	FPoseWingEffector AddFlapAngle(const float Delta) const
	{
		FPoseWingEffector Copy = *this;
		Copy.FlapAngle += Delta;
		return Copy;
	}
	
	FPoseWingEffector SetOpenness(const float NewOpenness) const
	{
		FPoseWingEffector Copy = *this;
		Copy.Openness = NewOpenness;
		return Copy;
	}
	FPoseWingEffector SetFlapAngle(const float NewFlap) const
	{
		FPoseWingEffector Copy = *this;
		Copy.FlapAngle = NewFlap;
		return Copy;
	}
};
