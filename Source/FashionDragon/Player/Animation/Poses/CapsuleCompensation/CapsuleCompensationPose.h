#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FCapsuleCompensationPose final : public FProceduralPose
{
public:
	explicit FCapsuleCompensationPose(UDragonAnimInstance* Anim);
};

class FCapsuleCompensationDriverRoot final : public FProceduralBoneDriver
{
	FVector LastFrameOffset = FVector::ZeroVector;
	
public:
	FCapsuleCompensationDriverRoot(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone)
	{
		BlendMode = EDriverBlend::AlwaysActive;
	}

	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
