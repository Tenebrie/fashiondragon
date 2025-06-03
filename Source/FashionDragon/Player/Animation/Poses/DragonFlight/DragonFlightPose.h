#pragma once
#include <typeinfo>

#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralWingDriver.h"

enum class EDragonFlightState
{
	Pushing,
	Retracting,
};

class FDragonFlightWingDriver final : public FProceduralWingDriver
{
	virtual FDragonWingStateData GetRawStateData() const override;
	
public:
	FDragonFlightWingDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledWing* ControlledWing
	): FProceduralWingDriver(AnimInstance, ControlledWing) {}

	EDragonFlightState FlightState = EDragonFlightState::Pushing;

	virtual void Tick(float DeltaTime) override;
	void AdvanceState();
};

class FDragonFlightPose final : public FProceduralPose
{
	FDragonFlightWingDriver* LeftWingDriver;
	FDragonFlightWingDriver* RightWingDriver;
public:
	explicit FDragonFlightPose(UDragonAnimInstance* Anim);

	virtual void ResetState() override;
};
