#pragma once
#include "ControlledLeg.h"
#include "Structs/PoseEffector.h"

class FControlledBone
{
public:
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
};

/**
 * @brief Abstract base class for procedural bone drivers
 */
class FAbstractProceduralDriver
{
protected:
	UDragonAnimInstance* AnimInstance;
	FControlledBone* Bone;
	float CyclePosition = 0.0f;
	float VisualCyclePosition = 0.0f;

	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector PositionFrom = FVector(0.0f, 0.0f, 0.0f);
	FRotator RotationFrom = FRotator(0.0f, 0.0f, 0.0f);

	FVector ArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector ArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
	
	// virtual void AdvanceState();
	// virtual std::pair<FVector, FRotator> GetTargetPosition() const;
	
public:
	virtual ~FAbstractProceduralDriver() = default;

	FAbstractProceduralDriver(UDragonAnimInstance* AnimInstance, FControlledBone* ControlledBone);

	virtual void Tick(float DeltaTime);
	// virtual void RecalculatePose();

	// virtual void SyncStateFrom(FAbstractProceduralDriver* TargetDriver);

	/**
	 * @brief Converts the current state of the leg driver to an effector.
	 * Effector is a struct defining the leg transformation that needs to be applied.
	 * The effectors are positional, and provided state includes all previous effectors applied.
	 */
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const float BlendAlpha);

	FControlledBone* GetBone() const { return Bone; }
	FVector GetPosition() const { return Position; }
	FRotator GetRotation() const { return Rotation; }
	float GetCyclePosition() const { return CyclePosition; }
	float GetVisualCyclePosition() const { return VisualCyclePosition; }
};