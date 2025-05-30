#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"
#include "FashionDragon/Utils/PerlinNoiseGenerator.h"

class FDragonRandomSwayDriverHips final : public FProceduralBoneDriver
{
	FPerlinNoiseGenerator RotationSwayNoise = FPerlinNoiseGenerator(5.0f, 0.5f);
public:
	FDragonRandomSwayDriverHips(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone)
	{
	}

	virtual void Tick(float DeltaTime) override;
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
