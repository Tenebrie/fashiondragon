#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

class FDragonDriverTurnToMovement final : public FProceduralBoneDriver
{
	FVector DragDirection = FVector::ZeroVector;
public:
	FDragonDriverTurnToMovement(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone)
	{
	}

	virtual void Tick(float DeltaTime) override;
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
