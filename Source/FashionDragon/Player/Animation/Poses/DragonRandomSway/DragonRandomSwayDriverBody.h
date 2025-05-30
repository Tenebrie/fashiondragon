#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"
#include "FashionDragon/Utils/PerlinNoiseGenerator.h"

class FDragonRandomSwayDriverBody final : public FProceduralBoneDriver
{
	float BreathCycle = 0.0f;
	FPerlinNoiseGenerator PositionSwayNoise = FPerlinNoiseGenerator(20.0f, 0.15f);
	FPerlinNoiseGenerator RotationSwayNoise = FPerlinNoiseGenerator(1.0f, 0.07f);
public:
	FDragonRandomSwayDriverBody(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone)
	{
	}

	virtual void Tick(float DeltaTime) override;
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
