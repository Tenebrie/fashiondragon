#pragma once
#include "ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledLeg.h"

struct FDragonWalkStateData
{
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

	mutable FVector StartArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	mutable FVector StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
	mutable FVector EndArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	mutable FVector EndArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
};

/**
 * @brief Abstract base class for procedural leg drivers
 */
class FProceduralLegDriver : public FProceduralBoneDriver
{
protected:
	FControlledLeg* Leg;

	float CycleDuration = 1.0f;
	FVector LockedWorldPosition = FVector(0.0f, 0.0f, 0.0f);
	FRotator LockedWorldRotation = FRotator(0.0f, 0.0f, 0.0f);

	virtual void SnapToLockedPosition();

	virtual void AdvanceState();
	virtual FDragonWalkStateData GetTargetPosition() const;
	
public:
	FProceduralLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	virtual void RecalculatePose(const float DeltaTime);

	bool LockWorldGroundPosition(const bool KeepCycle = false);

	/**
	 * @brief Syncs the current state of the leg driver with another.
	 * Intended to be used when starting transition between two poses.
	 * 
	 * Default implementation assumes the other driver has the same duration.
	 * @param TargetDriver Compatible driver to sync with
	 */
	virtual void SyncStateFrom(FProceduralLegDriver* TargetDriver);
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;

	FControlledLeg* GetLeg() const { return Leg; }

	// TODO: Extract into a separate interface
	ELegWalkingState WalkingState = ELegWalkingState::Relaxed;
	virtual void SetWalkingState(ELegWalkingState NewState, const bool KeepCycle = false);
};