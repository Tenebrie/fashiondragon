#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

class FDragonDriverTurnToMovement final : public FProceduralBoneDriver
{
	FVector AxisMask = FVector(1.0f, 0.0f, 0.0f);
	FRotator DragDirection = FRotator::ZeroRotator;
	FRotator LastSeenRotation = FRotator::ZeroRotator;
public:
	FDragonDriverTurnToMovement(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone)
	{
		BlendMode = EDriverBlend::EaseOut;
		BlendSpeed = 0.5f;
	}

	void SetAxisMask(const float X, const float Y, const float Z) { AxisMask = FVector(X, Y, Z); }
	void SetAxisMask(const FVector& NewAxisMask) { AxisMask = NewAxisMask; }
	virtual void Tick(float DeltaTime) override;
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
