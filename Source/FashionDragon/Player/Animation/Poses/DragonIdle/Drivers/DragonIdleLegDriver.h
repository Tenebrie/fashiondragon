#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Utils/DragonIdleLegStateChangedDelegate.h"

class FDragonWalkLegDriver;

class FDragonIdleLegDriver final : public FProceduralLegDriver
{
public:
	FDragonIdleLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg):
		FProceduralLegDriver(AnimInstance, ControlledLeg) {}

	virtual void AdvanceState() override {}
	virtual void Tick(float DeltaTime) override;
	virtual FDragonWalkStateData GetRawWalkStateData() const override;

	FDragonIdleLegDriverStateChangedDelegate OnIdleStateChanged;
	
	ELegIdleState IdleState = ELegIdleState::Relaxed;
	void SetIdleState(ELegIdleState NewState, bool SkipBroadcast = false);
	void SyncIdleStateFrom(const FDragonWalkLegDriver* TargetDriver);

private:
	FDragonWalkStateData ArticulatedReturnData = {};
};
