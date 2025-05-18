#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Delegates/DragonIdleLegStateChangedDelegate.h"

class FDragonWalkLegDriver;

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
	void SetIdleState(ELegIdleState NewState, bool SkipBroadcast = false);
};
