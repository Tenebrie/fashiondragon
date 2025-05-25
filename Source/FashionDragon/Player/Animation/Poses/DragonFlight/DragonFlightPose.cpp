#include "DragonFlightPose.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Enums/DriverLayer.h"

FDragonWingStateData FDragonFlightWingDriver::GetRawStateData() const
{
	const std::map<EDragonFlightState, FDragonWingStateData> AnimData =
	{
		{ EDragonFlightState::Pushing,
			{
				.Flap = FArticulatedValue(1.2f),
				.Openness = FArticulatedValue(1.0f),
				.Duration = 0.4f,
				.TransitionSpeed = 300.0f,
			}
		},
		{ EDragonFlightState::Retracting,
			{
				.Flap = FArticulatedValue(-1.2f),
				.Openness = FArticulatedValue(1.0f, -1.2f, -0.5f),
				.Duration = 0.9f,
				.TransitionSpeed = 8.0f,
			}
		},
	};
	
	return AnimData.at(FlightState);
}

void FDragonFlightWingDriver::Tick(const float DeltaTime)
{
	FProceduralWingDriver::Tick(DeltaTime);

	if (CyclePosition >= CycleDuration)
	{
		CyclePosition = 0.0f;
		VisualCyclePosition = 0.0f;
		AdvanceState();
		CycleDuration = GetRawStateData().Duration;
	}
}

void FDragonFlightWingDriver::AdvanceState()
{
	switch (FlightState)
	{
	case EDragonFlightState::Pushing:
		FlightState = EDragonFlightState::Retracting;
		break;
	case EDragonFlightState::Retracting:
		FlightState = EDragonFlightState::Pushing;
		break;
	}
	StartedFlapFrom = Wing->Flap;
	StartedOpennessFrom = Wing->Openness;
}

FDragonFlightPose::FDragonFlightPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LeftWingDriver = new FDragonFlightWingDriver(Anim, Anim->LeftWing.GetBone(EDriverLayer::Primary));
	RightWingDriver = new FDragonFlightWingDriver(Anim, Anim->RightWing.GetBone(EDriverLayer::Primary));
	WingDrivers = {
		LeftWingDriver,
		RightWingDriver,
	};
}

void FDragonFlightPose::ResetState()
{
	FProceduralPose::ResetState();
}
