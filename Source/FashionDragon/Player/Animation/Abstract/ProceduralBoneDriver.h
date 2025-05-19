#pragma once
#include "BaseDriver.h"

#include "FashionDragon/Player/Animation/Structs/PoseEffectorContext.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledBone.h"
#include "FashionDragon/Player/Animation/Structs/PoseEffector.h"

/**
 * @brief Abstract base class for procedural bone drivers
 */
class FProceduralBoneDriver: protected FBaseDriver
{
protected:
	FControlledBone* Bone;

	FVector DesiredPosition = FVector::ZeroVector;
	FRotator DesiredRotation = FRotator::ZeroRotator;
	FVector PositionFrom = FVector::ZeroVector;
	FRotator RotationFrom = FRotator::ZeroRotator;

	FVector ArticulationPosition = FVector::ZeroVector;
	FVector ArticulationRotation = FVector::ZeroVector;
	
public:
	FProceduralBoneDriver(UDragonAnimInstance* AnimInstance, FControlledBone* ControlledBone):
		FBaseDriver(AnimInstance), Bone(ControlledBone) {}

	virtual void Tick(float DeltaTime);
	virtual void ResetState();

	/**
	 * @brief Converts the current state of the leg driver to an effector.
	 * Effector is a struct defining the leg transformation that needs to be applied.
	 * The effectors are positional, and provided state includes all previous effectors applied.
	 */
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context);

	/**
	 * @brief Similar to `ToEffector`, but the resulting positions are NOT persistent across frames.
	 * This effector can apply any transformation that will not affect the persistent state.
	 */
	virtual FPoseEffector ToPostProcessEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context);

	FControlledBone* GetBone() const { return Bone; }
	FVector GetDesiredPosition() const { return DesiredPosition; }
	FRotator GetDesiredRotation() const { return DesiredRotation; }
};