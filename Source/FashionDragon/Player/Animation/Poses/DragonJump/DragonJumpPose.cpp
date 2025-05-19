#include "DragonJumpPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonJumpLegDriver::FDragonJumpLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
	: FProceduralLegDriver(AnimInstance, ControlledLeg)
{
}

void FDragonJumpLegDriver::Tick(float DeltaTime)
{
	const auto Player = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (Player->IsChargingJump && JumpingState == ELegJumpState::Charging)
	{
		DeltaTime = 0.f;
		CyclePosition = Player->JumpCharge;
		VisualCyclePosition = Player->JumpCharge;
	}

	if (!Player->IsChargingJump && JumpingState == ELegJumpState::Charging)
	{
		SetJumpState(ELegJumpState::Pushing);
	}
	if (JumpingState == ELegJumpState::Pushing)
	{
		DeltaTime *= 5.0f;
	}
	if (JumpingState == ELegJumpState::Retracting)
	{
		DeltaTime *= 3.0f;
	}
	
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
			}
		},
		{ ELegJumpState::Retracting,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 150.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
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
