#include "DragonIdlePose.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

// ============================================================================
// Leg Driver
// ============================================================================

FDragonIdleLegDriver::FDragonIdleLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FProceduralLegDriver(AnimInstance, ControlledLeg)
{}

void FDragonIdleLegDriver::Tick(const float DeltaTime)
{
	FProceduralLegDriver::Tick(DeltaTime);
	if (IdleState == ELegIdleState::ArticulatedReturn && Leg->Position.Size() < 5.f)
	{
		SetIdleState(ELegIdleState::Relaxed);
	}
}

FDragonWalkStateData FDragonIdleLegDriver::GetTargetPosition() const
{
	if (IdleState == ELegIdleState::ArticulatedReturn) { return ArticulatedReturnData; }
	
	return {
		.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
		.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
		.LinearForce = 1000.0f,
		.AngularForce = 360.0f,
		.StateDuration = 0.2f,
	};
}

void FDragonIdleLegDriver::SyncIdleStateFrom(const FDragonWalkLegDriver* TargetDriver)
{
	ResetState();
	if (TargetDriver->WalkingState == ELegWalkingState::Planted)
	{
		LockRealWorldPosition();
		SetIdleState(ELegIdleState::Planted);
	}
	else
		SetIdleState(ELegIdleState::ArticulatedReturn);
}

void FDragonIdleLegDriver::SetIdleState(const ELegIdleState NewState)
{
	if (NewState == ELegIdleState::Planted)
		SetWalkingState(ELegWalkingState::Planted);
	else
		SetWalkingState(ELegWalkingState::Relaxed);
	
	if (NewState == ELegIdleState::ArticulatedReturn)
	{
		constexpr auto Speed = 1200.0f;
		constexpr float Duration = 0.5f;

		ArticulatedReturnData = {
			.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
			.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
			.LinearForce = Speed,
			.AngularForce = Speed,
			.StateDuration = Duration,
			.StartArticulationPosition = FVector(0.0f, 0.0f, 0.0f),
			.EndArticulationPosition = FVector(0.0f, 0.0f, 35.f),
		};
	}
	OnIdleStateChanged.Broadcast(IdleState, NewState);
	IdleState = NewState;
}

// ============================================================================
// Pose
// ============================================================================

FDragonIdlePose::FDragonIdlePose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BodyDriver = new FDragonIdleBodyDriver(Anim, Anim->ControlledBody);
	HipsDriver = new FDragonIdleBodyDriver(Anim, Anim->ControlledHips);
	FProceduralPose::BodyDriver = BodyDriver;
	FProceduralPose::HipsDriver = HipsDriver;

	LeftLegDriver = new FDragonIdleLegDriver(Anim, Anim->BackLeftLeg);
	RightLegDriver = new FDragonIdleLegDriver(Anim, Anim->BackRightLeg);
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};

	LeftLegDriver->OnIdleStateChanged.AddLambda([this](const ELegIdleState OldState, const ELegIdleState)
	{
		if (OldState == ELegIdleState::ArticulatedReturn && RightLegDriver->IdleState == ELegIdleState::Planted)
		{
			RightLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
		}
	});
	RightLegDriver->OnIdleStateChanged.AddLambda([this](const ELegIdleState OldState, const ELegIdleState)
	{
		if (OldState == ELegIdleState::ArticulatedReturn && LeftLegDriver->IdleState == ELegIdleState::Planted)
		{
			LeftLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
		}
	});
}

void FDragonIdlePose::SyncStateFrom(const FDragonWalkPose* TargetPose) const
{
	LeftLegDriver->SyncIdleStateFrom(TargetPose->LeftLegDriver);
	RightLegDriver->SyncIdleStateFrom(TargetPose->RightLegDriver);
}
