#pragma once
#include "ControlledLeg.h"
#include "Structs/PoseEffector.h"

#define LEG_POSE(Vector, Rotator) std::make_pair(FVector Vector, FRotator Rotator)

/**
 * @brief Abstract base class for procedural leg drivers
 */
class FAbstractProceduralLegDriver
{
protected:
	UDragonAnimInstance* AnimInstance;
	FControlledLeg* Leg;
	float CyclePosition = 0.0f;
	float VisualCyclePosition = 0.0f;

	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector PositionFrom = FVector(0.0f, 0.0f, 0.0f);
	FRotator RotationFrom = FRotator(0.0f, 0.0f, 0.0f);
	FVector LockedWorldPosition = FVector(0.0f, 0.0f, 0.0f);
	FRotator LockedWorldRotation = FRotator(0.0f, 0.0f, 0.0f);

	FVector ArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector ArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
	
	virtual void SnapToLockedPosition();

	virtual void AdvanceState();
	virtual std::pair<FVector, FRotator> GetTargetPosition() const;
	
public:
	virtual ~FAbstractProceduralLegDriver() = default;

	FAbstractProceduralLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime);
	virtual void RecalculatePose();

	void LockRealWorldPosition();
	void LockTargetWorldPosition();

	/**
	 * @brief Syncs the current state of the leg driver with another.
	 * Intended to be used when starting transition between two poses.
	 * 
	 * Default implementation assumes the other driver has the same duration.
	 * @param TargetDriver Compatible driver to sync with
	 */
	virtual void SyncStateFrom(FAbstractProceduralLegDriver* TargetDriver);

	/**
	 * @brief Converts the current state of the leg driver to an effector.
	 * Effector is a struct defining the leg transformation that needs to be applied.
	 * The effectors are positional, and provided state includes all previous effectors applied.
	 */
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const float BlendAlpha);

	FControlledLeg* GetLeg() const { return Leg; }
	FVector GetPosition() const { return Position; }
	FRotator GetRotation() const { return Rotation; }
	float GetCyclePosition() const { return CyclePosition; }
	float GetVisualCyclePosition() const { return VisualCyclePosition; }

	// TODO: Extract into a separate interface
	ELegWalkingState WalkingState = ELegWalkingState::Relaxed;
	virtual void SetWalkingState(ELegWalkingState NewState, const bool KeepCycle = false);
};