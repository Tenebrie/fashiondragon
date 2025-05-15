#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonWalkPose;
class FDragonWalkLegDriver;

class FDragonIdleBodyDriver final : public FProceduralBoneDriver
{
public:
	FDragonIdleBodyDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* ControlledBone
	): FProceduralBoneDriver(AnimInstance, ControlledBone) {}
};

enum class ELegIdleState
{
	Relaxed,
	Planted,
	ArticulatedReturn,
};

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FDragonIdleLegDriverStateChangedDelegate,
	ELegIdleState OldState,
	ELegIdleState NewState
);

/**
 * @brief Idle animation driver
 */
class FDragonIdleLegDriver final : public FProceduralLegDriver
{
public:
	FDragonIdleLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;

	FDragonIdleLegDriverStateChangedDelegate OnIdleStateChanged;

	virtual FDragonWalkStateData GetTargetPosition() const override;
	FDragonWalkStateData ArticulatedReturnData = {};
	void SyncIdleStateFrom(const FDragonWalkLegDriver* TargetDriver);

	ELegIdleState IdleState = ELegIdleState::Relaxed;
	void SetIdleState(ELegIdleState NewState);
};

/**
 * @brief Idle animation pose (no movement)
 */
class FDragonIdlePose final : public FProceduralPose
{
public:
	explicit FDragonIdlePose(UDragonAnimInstance* Anim);

	FDragonIdleBodyDriver* BodyDriver;
	FDragonIdleBodyDriver* HipsDriver;
	FDragonIdleLegDriver* LeftLegDriver;
	FDragonIdleLegDriver* RightLegDriver;

	void SyncStateFrom(const FDragonWalkPose* TargetPose) const;
};