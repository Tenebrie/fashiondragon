#include "DragonJumpPose.h"

#include <map>

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

void FDragonJumpBodyDriver::Tick(const float DeltaTime)
{
	FProceduralBoneDriver::Tick(DeltaTime);
	BlendAlpha = FMath::FInterpTo(BlendAlpha, TargetBlendAlpha, DeltaTime, 0.1f);
	if (TargetBlendAlpha > BlendAlpha)
	{
		BlendAlpha = TargetBlendAlpha;
	}

	DesiredForce = 2.f;
	if (JumpingState == ELegJumpState::Pushing)
	{
		DesiredPosition = FVector(0.0f, 0.0f, 100.0f);
		DesiredForce = 1.f;
	}
	else if (JumpingState == ELegJumpState::Retracting)
	{
		DesiredPosition = FVector(0.0f, 0.0f, 100.0f);
		DesiredForce = 4.f;
	}
	else if (JumpingState == ELegJumpState::Landing)
	{
		DesiredPosition = FVector(0.0f, 0.0f, 100.0f);
		DesiredForce = 4.f;
	}
	else if (JumpingState == ELegJumpState::Impact)
	{
		DesiredPosition = FVector(0.0f, 0.0f, -75.0f);
		DesiredRotation = FRotator(0.0f, 0.0f, 7.5f);
		DesiredForce = 10.f;
	}
	else
	{
		DesiredPosition = FVector::ZeroVector;
		DesiredRotation = FRotator::ZeroRotator;
	}
}

void FDragonJumpBodyDriver::SetJumpState(const ELegJumpState NewJumpState)
{
	JumpingState = NewJumpState;
}

FPoseEffector FDragonJumpBodyDriver::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	return FProceduralBoneDriver::ToEffector(BaseEffector, Context);
}

FDragonJumpLegDriver::FDragonJumpLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
	: FProceduralLegDriver(AnimInstance, ControlledLeg)
{
}

void FDragonJumpLegDriver::Tick(const float DeltaTime)
{
	FProceduralLegDriver::Tick(DeltaTime);

	if (JumpingState == ELegJumpState::Retracting && AnimInstance->GetOwningActor()->GetVelocity().Z < 0.0f && Leg->GetPlantedWorldPosition(500).GroundHit)
	{
		SetJumpState(ELegJumpState::DelayedLanding);
	}
	if (JumpingState == ELegJumpState::Landing && Leg->GetPlantedWorldPosition(5).GroundHit)
	{
		SetJumpState(ELegJumpState::Impact);
	}
}

void FDragonJumpLegDriver::AdvanceState()
{
	switch (JumpingState)
	{
	case ELegJumpState::Pushing:
		SetJumpState(ELegJumpState::Retracting);
		break;
	// case ELegJumpState::Retracting:
	// 	SetJumpState(ELegJumpState::Landing);
	// 	break;
	case ELegJumpState::DelayedLanding:
		SetJumpState(ELegJumpState::Landing);
		break;
	case ELegJumpState::Impact:
		SetJumpState(ELegJumpState::Relaxed);
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
		{ ELegJumpState::Pushing,
			{
				.TargetPosition = FVector(0.0f, 0.0f, -1000.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 30.0f),
				.Duration = 0.10f + JumpLagBehind,
			}
		},
		{ ELegJumpState::Retracting,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 150.0f) + JumpPositionOffset,
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
				.Duration = 0.50f,
			}
		},
		{ ELegJumpState::DelayedLanding,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 150.0f) + JumpPositionOffset,
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
				.Duration = JumpLagBehind,
			}
		},
		{ ELegJumpState::Landing,
			{
				.TargetPosition = FVector(0.0f, 0.0f, -50.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.Duration = 0.50f,
			}
		},
		{ ELegJumpState::Impact,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.Duration = 0.30f,
			}
		},
		{ ELegJumpState::Relaxed,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 0.0f,
				.AngularForce = 0.0f,
				.Duration = 1000.00f,
			}
		},
	};
	return AnimData.at(JumpingState);
}

void FDragonJumpLegDriver::SetJumpState(const ELegJumpState NewJumpState)
{
	SetWalkingState(ELegWalkingState::Raised);
	OnJumpStateChanged.Broadcast(JumpingState, NewJumpState);
	JumpingState = NewJumpState;
	CyclePosition = 0.0f;
	VisualCyclePosition = 0.0f;
	CycleDuration = GetTargetPosition().Duration;
	PositionFrom = Leg->Position;
	RotationFrom = Leg->Rotation;
}

void FDragonJumpLegDriver::UpdateRandomness(const bool LagBehind)
{
	if (LagBehind)
		JumpLagBehind = FMath::FRand() * 0.05f + 0.075f;
	else
		JumpLagBehind = 0.0f;
	JumpPositionOffset = FVector(FMath::FRand() * 50.0f, FMath::FRand() * 50.0f, FMath::FRand() * 50.0f) - FVector(25.0f, 25.0f, 25.0f);
}

FDragonJumpPose::FDragonJumpPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BodyDriver = new FDragonJumpBodyDriver(Anim, &Anim->ControlledBody, EBodyDriverGroup::Jump);
	BodyDrivers = { BodyDriver };

	LeftLegDriver = new FDragonJumpLegDriver(Anim, Anim->BackLeftLeg);
	RightLegDriver = new FDragonJumpLegDriver(Anim, Anim->BackRightLeg);
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};

	LeftLegDriver->OnJumpStateChanged.AddLambda([this](const ELegJumpState, const ELegJumpState NewState)
	{
		BodyDriver->SetJumpState(NewState);
	});
	RightLegDriver->OnJumpStateChanged.AddLambda([this](const ELegJumpState, const ELegJumpState NewState)
	{
		BodyDriver->SetJumpState(NewState);
	});
}

void FDragonJumpPose::ResetState()
{
	LeftLegDriver->UpdateRandomness(SwitchJumpingLeg);
	RightLegDriver->UpdateRandomness(!SwitchJumpingLeg);
	LeftLegDriver->SetJumpState(ELegJumpState::Pushing);
	RightLegDriver->SetJumpState(ELegJumpState::Pushing);

	SwitchJumpingLeg = !SwitchJumpingLeg;
}
