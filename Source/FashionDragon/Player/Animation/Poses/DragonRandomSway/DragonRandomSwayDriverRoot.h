#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"
#include "FashionDragon/Utils/PerlinNoiseGenerator.h"

class FDragonRandomSwayDriverRoot final : public FProceduralBoneDriver
{
	float BreathCycle = 0.0f;
	FPerlinNoiseGenerator PositionSwayNoise = FPerlinNoiseGenerator(20.0f, 0.15f);
	FPerlinNoiseGenerator RotationSwayNoise = FPerlinNoiseGenerator(1.0f, 0.07f);
public:
	FDragonRandomSwayDriverRoot(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone)
	{
	}

	void SetPositionAmplitude(const float Amplitude, const float Scale);
	void SetRotationAmplitude(const float Amplitude, const float Scale);
	virtual void Tick(float DeltaTime) override;
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
