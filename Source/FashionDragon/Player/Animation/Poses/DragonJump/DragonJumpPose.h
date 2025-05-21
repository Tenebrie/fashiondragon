#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

enum class ELegJumpState
{
	Pushing,
	Retracting,
	DelayedLanding,
	Landing,
	Impact,
	Relaxed,
};

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FDragonJumpLegDriverStateChangedDelegate,
	ELegJumpState OldState,
	ELegJumpState NewState
);

class FDragonJumpPose;

class FDragonJumpBodyDriver final : public FProceduralBoneDriver
{
	float TargetBlendAlpha = 1.0f;
public:
	FDragonJumpBodyDriver(
		UDragonAnimInstance* AnimInstance,
		TArray<FControlledBone*>* ControlledBones,
		const auto DriverGroup = EBodyDriverGroup::General
	): FProceduralBoneDriver(AnimInstance, ControlledBones, DriverGroup) {}
	
	virtual void Tick(float DeltaTime) override;
	
	ELegJumpState JumpingState = ELegJumpState::Relaxed;
	void SetJumpState(ELegJumpState NewJumpState);

	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
	virtual void SetBlendAlpha(const float NewBlendAlpha) override { TargetBlendAlpha = NewBlendAlpha; }
};

class FDragonJumpLegDriver final : public FProceduralLegDriver
{
	float JumpLagBehind = 0.0f;
	FVector JumpPositionOffset = FVector::ZeroVector;
public:
	FDragonJumpLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	
	virtual void AdvanceState() override;

	virtual FDragonWalkStateData GetRawWalkStateData() const override;

	ELegJumpState JumpingState = ELegJumpState::Relaxed;
	void SetJumpState(ELegJumpState NewJumpState);

	FDragonJumpLegDriverStateChangedDelegate OnJumpStateChanged;

	void UpdateRandomness(const bool LagBehind);
};

/**
 * @brief Jump preparation pose (jump button held)
 */
class FDragonJumpPose final : public FProceduralPose
{
	bool SwitchJumpingLeg = false;
	FDragonJumpBodyDriver* BodyDriver;
	FDragonJumpLegDriver* LeftLegDriver;
	FDragonJumpLegDriver* RightLegDriver;
public:
	explicit FDragonJumpPose(UDragonAnimInstance* Anim);

	virtual void ResetState() override;
};