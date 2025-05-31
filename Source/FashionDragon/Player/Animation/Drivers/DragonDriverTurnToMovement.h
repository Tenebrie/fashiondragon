#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

class FDragonDriverTurnToMovement final : public FProceduralBoneDriver
{
	FRotator DragDirection = FRotator::ZeroRotator;
	FRotator LastSeenRotation = FRotator::ZeroRotator;
public:
	FDragonDriverTurnToMovement(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone)
	{
	}

	virtual void Tick(float DeltaTime) override;
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
