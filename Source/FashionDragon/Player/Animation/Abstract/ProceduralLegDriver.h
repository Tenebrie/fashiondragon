#pragma once
#include "ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/ControlledLeg.h"

struct FDragonWalkStateData
{
	FVector TargetPosition;
	FRotator TargetRotation;
	/**
	 * Maximum force an effector may apply to the bone in this state.
	 * Effectively, maximum speed per second.
	 */
	float LinearForce;
	/**
	 * Maximum force an effector may apply to the bone's rotation in this state.
	 * Effectively, maximum speed (angles) per second.
	 */
	float AngularForce;
	/**
	 * Duration of the state in seconds.
	 */
	float StateDuration;

	FVector StartArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
	FVector EndArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector EndArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
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

	void LockRealWorldPosition();
	void LockTargetWorldPosition();

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