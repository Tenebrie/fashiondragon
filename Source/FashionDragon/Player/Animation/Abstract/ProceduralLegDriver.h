#pragma once
#include "ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledLeg.h"

struct FDragonWalkStateData;

/**
 * @brief Abstract base class for procedural leg drivers
 */
class FProceduralLegDriver : protected FBaseDriver
{
public:
	virtual void AdvanceState() = 0;
	virtual FDragonWalkStateData GetRawWalkStateData() const = 0;
	
	virtual void Tick(float DeltaTime);
	virtual void ResetState();

	void SyncStateFrom(const FProceduralLegDriver* TargetDriver);
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context);
	virtual FPoseEffector ToPostProcessEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) { return BaseEffector; }

	ELegWalkingState WalkingState = ELegWalkingState::Relaxed;
	void SetWalkingState(ELegWalkingState NewState, const bool KeepCycle = false);
	bool LockToWorldGround(const bool KeepCycle = false);

	FControlledLeg* GetLeg() const { return Leg; }
	FVector GetDesiredPosition() const { return DesiredPosition; }
	FRotator GetDesiredRotation() const { return DesiredRotation; }

	FProceduralLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
		: FBaseDriver(AnimInstance), Leg(ControlledLeg) {}

protected:
	FControlledLeg* Leg;
	float CyclePosition = 0.0f;
	float VisualCyclePosition = 0.0f;

	FVector DesiredPosition = FVector::ZeroVector;
	FRotator DesiredRotation = FRotator::ZeroRotator;
	FVector PositionFrom = FVector::ZeroVector;
	FRotator RotationFrom = FRotator::ZeroRotator;
	FVector ArticulationPosition = FVector::ZeroVector;
	FVector ArticulationRotation = FVector::ZeroVector;

	float CycleDuration = 1.0f;
	FVector LockedWorldPosition = FVector::ZeroVector;
	FRotator LockedWorldRotation = FRotator::ZeroRotator;
	
	void SnapToLockedPosition();
	void RecalculatePose(const float DeltaTime);

	FDragonWalkStateData GetTargetPosition() const;
	FDragonWalkStateData AlignPoseToInputDirection(FDragonWalkStateData PoseData) const;
};

inline void FProceduralLegDriver::ResetState() { CyclePosition = 0; VisualCyclePosition = 0; }

struct FDragonWalkStateData {
	mutable FVector TargetPosition;
	mutable FRotator TargetRotation;
	/**
	 * Maximum force an effector may apply to the bone in this state.
	 * Effectively, maximum speed per second.
	 */
	float LinearForce = 10000.0f;
	/**
	 * Maximum force an effector may apply to the bone's rotation in this state.
	 * Effectively, maximum speed (angles) per second.
	 */
	float AngularForce = 360.0f;
	/**
	 * Duration of the state in seconds.
	 * Should not be modified dynamically.
	 */
	float Duration = 1.0f;

	/**
	 * Playback speed. Can be safely modified dynamically.
	 */
	mutable float PlaybackSpeed = 1.0f;

	mutable FVector StartArticulationPosition = FVector::ZeroVector;
	mutable FVector StartArticulationRotation = FVector::ZeroVector;
	mutable FVector EndArticulationPosition = FVector::ZeroVector;
	mutable FVector EndArticulationRotation = FVector::ZeroVector;
};

