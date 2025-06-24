#include "DragonFlightPose.h"

#include "DragonFlightDriverLeg.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Enums/DriverLayer.h"
#include "FashionDragon/Player/InputHandlers/FlightHandler.h"

FDragonWingStateData FDragonFlightWingDriver::GetRawStateData() const
{
	const std::map<EDragonFlightState, FDragonWingStateData> AnimData =
	{
		{ EDragonFlightState::Gliding,
			{
				.Duration = 0.4f,
				.TransitionSpeed = 10.0f,
			}
		},
		{ EDragonFlightState::Pushing,
			{
				.FlapAngle = FArticulatedValue(1.0f),
				.TiltAngle = FArticulatedValue(0.0f),
				.FlightFoldState = FArticulatedValue(0.0f),
				.Duration = 0.4f,
				.TransitionSpeed = 300.0f,
			}
		},
		{ EDragonFlightState::Retracting,
			{
				.FlapAngle = FArticulatedValue(-1.0f),
				.TiltAngle = FArticulatedValue(1.0f),
				.FlightFoldState = FArticulatedValue(0.0f, -1.2f, -0.5f),
				.Duration = 0.9f,
				.TransitionSpeed = 8.0f,
			}
		},
		{ EDragonFlightState::WingsFolded,
			{
				.FlapAngle = FArticulatedValue(0.0f),
				.FlightFoldState = FArticulatedValue(1.0f),
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

	const auto FlightHandler = AnimInstance->GetCharacter()->FlightHandler;

	if (FlightHandler->IsFoldingWings())
	{
		SetFlightState(EDragonFlightState::WingsFolded);
	}
	else if ((FlightState == EDragonFlightState::Gliding || FlightState == EDragonFlightState::WingsFolded) && FlightHandler->IsFlapping())
	{
		SetFlightState(EDragonFlightState::Pushing);
	}
	else if (!FlightHandler->IsFlapping())
	{
		SetFlightState(EDragonFlightState::Gliding);
	}

	if (CyclePosition >= GetRawStateData().Duration)
	{
		AdvanceState();
	}
}

void FDragonFlightWingDriver::SetFlightState(const EDragonFlightState State)
{
	if (FlightState == State)
		return;
	
	CyclePosition = 0.0f;
	FlightState = State;
	StartingState = Wing->State;
}

void FDragonFlightWingDriver::AdvanceState()
{
	switch (FlightState)
	{
	case EDragonFlightState::Gliding:
		break;
	case EDragonFlightState::Pushing:
		SetFlightState(EDragonFlightState::Retracting);
		break;
	case EDragonFlightState::Retracting:
		SetFlightState(EDragonFlightState::Pushing);
		break;
	case EDragonFlightState::WingsFolded:
		break;
	}
}

FDragonFlightPose::FDragonFlightPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LegDrivers = {
		new FDragonFlightDriverLeg(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Primary)),
		new FDragonFlightDriverLeg(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Primary)),
	};
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
