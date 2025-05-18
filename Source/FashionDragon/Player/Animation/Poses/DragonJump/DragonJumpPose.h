#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

enum class ELegJumpState
{
	Charging,
	Pushing,
	Retracting,
	Landing,
};

/**
 * @brief Jump preparation animation leg driver
 */
class FDragonJumpLegDriver final : public FProceduralLegDriver
{
public:
	FDragonJumpLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	
	virtual void AdvanceState() override;

	virtual FDragonWalkStateData GetTargetPosition() const override;

	ELegJumpState JumpingState = ELegJumpState::Charging;
	void SetJumpState(ELegJumpState NewJumpState);
};

/**
 * @brief Jump preparation pose (jump button held)
 */
class FDragonJumpPose final : public FProceduralPose
{
public:
	explicit FDragonJumpPose(UDragonAnimInstance* Anim);

	virtual void ResetState() override;
};