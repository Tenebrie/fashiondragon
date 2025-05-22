#pragma once
#include "BaseDriver.h"

#include "FashionDragon/Player/Animation/Structs/PoseEffectorContext.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledBone.h"
#include "FashionDragon/Player/Animation/Structs/PoseEffector.h"

/**
 * @brief Abstract base class for procedural bone drivers
 */
class FProceduralBoneDriver: public FBaseDriver
{
protected:
	FControlledBone* Bone;
	
	float DesiredForce = 200.0f;
	FVector DesiredPosition = FVector::ZeroVector;
	FRotator DesiredRotation = FRotator::ZeroRotator;
	FVector PositionFrom = FVector::ZeroVector;
	FRotator RotationFrom = FRotator::ZeroRotator;

	FVector ArticulationPosition = FVector::ZeroVector;
	FVector ArticulationRotation = FVector::ZeroVector;
	
public:
	FProceduralBoneDriver(UDragonAnimInstance* AnimInstance, FControlledBone* Bone):
		FBaseDriver(AnimInstance), Bone(Bone)
	{
	}

	virtual void Tick(float DeltaTime);
	virtual void ResetState();

	/**
	 * @brief Converts the current state of the leg driver to an effector.
	 * Effector is a struct defining the leg transformation that needs to be applied.
	 * The effectors are positional, and provided state includes all previous effectors applied.
	 */
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context);

	FControlledBone* GetBone() const { return Bone; }
	FVector GetDesiredPosition() const { return DesiredPosition; }
	FRotator GetDesiredRotation() const { return DesiredRotation; }
};
