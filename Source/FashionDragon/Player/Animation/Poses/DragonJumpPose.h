#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

const enum ELegJumpState
{
	Charging = 0,
	Pushing = 1,
	Retracting = 2,
	Landing = 3,
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

	ELegJumpState JumpingState = Charging;
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