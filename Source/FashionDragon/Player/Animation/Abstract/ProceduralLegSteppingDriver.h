#pragma once
#include "ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledLeg.h"

struct FDragonWalkStateData;

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FDragonLegDriverWalkStateChangedDelegate,
	ELegWalkingState OldState,
	ELegWalkingState NewState
);

/**
 * @brief Abstract base class for procedural leg drivers
 */
class FProceduralLegSteppingDriver : public FProceduralLegDriver
{
protected:
	FVector LockedWorldPosition = FVector::ZeroVector;
	FRotator LockedWorldRotation = FRotator::ZeroRotator;
	
	void SnapToLockedPosition();
	void RecalculatePose(const float DeltaTime);

	FDragonWalkStateData GetTargetPosition() const;
	FDragonWalkStateData AlignPoseToInputDirection(FDragonWalkStateData PoseData) const;
	
public:
	FProceduralLegSteppingDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
		: FProceduralLegDriver(AnimInstance, ControlledLeg) {}
	
	virtual void AdvanceState() {};
	virtual FDragonWalkStateData GetRawWalkStateData() const;

	virtual void Tick(float DeltaTime) override;

	void SyncStateFrom(const FProceduralLegSteppingDriver* TargetDriver);
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
	
	ELegWalkingState WalkingState = ELegWalkingState::Relaxed;
	void SetWalkingState(ELegWalkingState NewState);
	bool LockToWorldGround();
	FDragonLegDriverWalkStateChangedDelegate OnWalkStateChanged;
};

struct FDragonWalkStateData {
	mutable FVector TargetPosition;
	mutable FRotator TargetRotation;
	float LinearForce = 1.0f;
	float AngularForce = 1.0f;
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

