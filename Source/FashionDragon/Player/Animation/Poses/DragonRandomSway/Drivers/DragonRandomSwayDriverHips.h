#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"
#include "FashionDragon/Utils/PerlinNoiseGenerator.h"

class FDragonRandomSwayDriverHips final : public FProceduralBoneDriver
{
	FPerlinNoiseGenerator RotationSwayNoise = FPerlinNoiseGenerator(5.0f, 0.5f);
public:
	FDragonRandomSwayDriverHips(UDragonAnimInstance* AnimInstance, TArray<FControlledBone*>* ControlledBones, const int DriverGroup)
		: FProceduralBoneDriver(AnimInstance, ControlledBones, DriverGroup)
	{
	}

	virtual void Tick(float DeltaTime) override;
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
