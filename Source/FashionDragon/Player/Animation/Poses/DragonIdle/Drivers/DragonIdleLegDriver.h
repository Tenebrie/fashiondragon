#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegSteppingDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Utils/DragonIdleLegStateChangedDelegate.h"

class FDragonIdleLegDriver final : public FProceduralLegSteppingDriver
{
public:
	FDragonIdleLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg):
		FProceduralLegSteppingDriver(AnimInstance, ControlledLeg) {}

	virtual void AdvanceState() override {}
	virtual void Tick(float DeltaTime) override;
	virtual FDragonWalkStateData GetRawWalkStateData() const override;

	FDragonIdleLegDriverStateChangedDelegate OnIdleStateChanged;
	
	ELegIdleState IdleState = ELegIdleState::Relaxed;
	void SetIdleState(ELegIdleState NewState, bool SkipBroadcast = false);
	template<typename DriverT>
	void SyncIdleStateFrom(const DriverT* TargetDriver);

private:
	FDragonWalkStateData ArticulatedReturnData = {};
};

