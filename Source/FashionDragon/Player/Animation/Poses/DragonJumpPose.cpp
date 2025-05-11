#include "DragonJumpPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonJumpLegDriver::FDragonJumpLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
	: FAbstractProceduralLegDriver(AnimInstance, ControlledLeg)
{
}

void FDragonJumpLegDriver::Tick(float DeltaTime)
{
	const auto Player = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (Player->IsChargingJump && JumpingState == Charging)
	{
		DeltaTime = 0.f;
		CyclePosition = Player->JumpCharge;
		VisualCyclePosition = Player->JumpCharge;
	}

	if (!Player->IsChargingJump && JumpingState == Charging)
	{
		SetJumpState(Pushing);
	}
	if (JumpingState == Pushing)
	{
		DeltaTime *= 5.0f;
	}
	if (JumpingState == Retracting)
	{
		DeltaTime *= 3.0f;
	}
	
	FAbstractProceduralLegDriver::Tick(DeltaTime);
}

void FDragonJumpLegDriver::AdvanceState()
{
	switch (JumpingState)
	{
	case Charging:
		SetJumpState(Pushing);
		break;
	case Pushing:
		SetJumpState(Retracting);
		break;
	case Retracting:
		SetJumpState(Landing);
		break;
	default:
		break;
	}
}

std::pair<FVector, FRotator> FDragonJumpLegDriver::GetTargetPosition() const
{
	const std::map<ELegJumpState, std::pair<FVector, FRotator>> LegStateToPosition =
	{
		{ Charging, LEG_POSE((0.0f, 0.0f, 150.0f), (0.0f, 0.0f, 0.0f)) },
		{ Pushing, LEG_POSE((0.0f, 0.0f, -150.0f), (0.0f, 0.0f, 30.0f)) },
		{ Retracting, LEG_POSE((0.0f, 0.0f, 150.0f), (0.0f, 0.0f, 60.0f)) },
		{ Landing, LEG_POSE((0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
	};
	
	return LegStateToPosition.at(JumpingState);
}

void FDragonJumpLegDriver::SetJumpState(const ELegJumpState NewJumpState)
{
	JumpingState = NewJumpState;
	VisualCyclePosition = 0.0f;
	CyclePosition = 0.0f;
	PositionFrom = Leg->Position;
	RotationFrom = Leg->Rotation;
}

FDragonJumpPose::FDragonJumpPose(UDragonAnimInstance* Anim): FAbstractProceduralPose(Anim)
{
	LegDrivers = {
		new FDragonJumpLegDriver(Anim, Anim->BackLeftLeg),
		new FDragonJumpLegDriver(Anim, Anim->BackRightLeg)
	};
}

void FDragonJumpPose::ResetState()
{
	static_cast<FDragonJumpLegDriver*>(LegDrivers[0])->SetJumpState(Pushing);
	static_cast<FDragonJumpLegDriver*>(LegDrivers[1])->SetJumpState(Pushing);
}
