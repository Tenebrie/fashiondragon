#include "DragonJumpPose.h"

#include <map>

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonJumpLegDriver::FDragonJumpLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
	: FProceduralLegDriver(AnimInstance, ControlledLeg)
{
}

void FDragonJumpLegDriver::Tick(float DeltaTime)
{
	FProceduralLegDriver::Tick(DeltaTime);
}

void FDragonJumpLegDriver::AdvanceState()
{
	switch (JumpingState)
	{
	case ELegJumpState::Charging:
		SetJumpState(ELegJumpState::Pushing);
		break;
	case ELegJumpState::Pushing:
		SetJumpState(ELegJumpState::Retracting);
		break;
	case ELegJumpState::Retracting:
		SetJumpState(ELegJumpState::Landing);
		break;
	default:
		break;
	}
}

// std::pair<FVector, FRotator> FDragonJumpLegDriver::GetTargetPosition() const
// {
// 	const std::map<ELegJumpState, std::pair<FVector, FRotator>> LegStateToPosition =
// 	{
// 		{ Charging, LEG_POSE((0.0f, 0.0f, 150.0f), (0.0f, 0.0f, 0.0f)) },
// 		{ Pushing, LEG_POSE((0.0f, 0.0f, -150.0f), (0.0f, 0.0f, 30.0f)) },
// 		{ Retracting, LEG_POSE((0.0f, 0.0f, 150.0f), (0.0f, 0.0f, 60.0f)) },
// 		{ Landing, LEG_POSE((0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
// 	};
// 	
// 	return LegStateToPosition.at(JumpingState);
// }

FDragonWalkStateData FDragonJumpLegDriver::GetRawWalkStateData() const
{
	const std::map<ELegJumpState, FDragonWalkStateData> AnimData =
	{
		{ ELegJumpState::Charging,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 150.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
			}
		},
		{ ELegJumpState::Pushing,
			{
				.TargetPosition = FVector(0.0f, 0.0f, -150.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 30.0f),
				.PlaybackSpeed = 5.0f,
			}
		},
		{ ELegJumpState::Retracting,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 150.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
				.PlaybackSpeed = 3.0f,
			}
		},
		{ ELegJumpState::Landing,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
			}
		},
	};
	return AnimData.at(JumpingState);
}

void FDragonJumpLegDriver::SetJumpState(const ELegJumpState NewJumpState)
{
	JumpingState = NewJumpState;
	VisualCyclePosition = 0.0f;
	CyclePosition = 0.0f;
	PositionFrom = Leg->Position;
	RotationFrom = Leg->Rotation;
}

FDragonJumpPose::FDragonJumpPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LegDrivers = {
		new FDragonJumpLegDriver(Anim, Anim->BackLeftLeg),
		new FDragonJumpLegDriver(Anim, Anim->BackRightLeg)
	};
}

void FDragonJumpPose::ResetState()
{
	static_cast<FDragonJumpLegDriver*>(LegDrivers[0])->SetJumpState(ELegJumpState::Pushing);
	static_cast<FDragonJumpLegDriver*>(LegDrivers[1])->SetJumpState(ELegJumpState::Pushing);
}
