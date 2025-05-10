#pragma once
#include "FashionDragon/Player/Animation/AbstractProceduralPose.h"

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
class FDragonJumpLegDriver final : public FAbstractProceduralLegDriver
{
public:
	FDragonJumpLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	
	virtual void AdvanceState() override;
	virtual std::pair<FVector, FRotator> GetTargetPosition() const override;

	ELegJumpState JumpingState = Charging;
	void SetJumpState(ELegJumpState NewJumpState);
};

/**
 * @brief Jump preparation pose (jump button held)
 */
class FDragonJumpPose final : public FAbstractProceduralPose
{
public:
	explicit FDragonJumpPose(
			UDragonAnimInstance* AnimInstance,
			FControlledLeg* LeftLeg,
			FControlledLeg* RightLeg
		);

	virtual void ResetState() override;
};