#include "DragonIdlePose.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Utils/Utils.h"

// ============================================================================
// Leg Driver
// ============================================================================

FDragonIdleLegDriver::FDragonIdleLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FProceduralLegDriver(AnimInstance, ControlledLeg)
{}

void FDragonIdleLegDriver::Tick(const float DeltaTime)
{
	FProceduralLegDriver::Tick(DeltaTime);
	if (IdleState == ELegIdleState::Relaxed)
	{
		LockWorldGroundPosition();
		SetIdleState(ELegIdleState::Planted);
	}
	if (IdleState == ELegIdleState::ArticulatedReturn && Leg->Position.Size() < 5.f)
	{
		LockWorldGroundPosition();
		SetIdleState(ELegIdleState::Planted);
	}

	const auto ShouldLeftDisconnect = Leg->Position.Size() > 150.0f || FUtils::GetRotatorDistance(Leg->Rotation) > 50.0f;
	if (IdleState != ELegIdleState::ArticulatedReturn && IdleState == ELegIdleState::Planted && ShouldLeftDisconnect)
	{
		SetIdleState(ELegIdleState::ArticulatedReturn);
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
		LockWorldGroundPosition();
		SetIdleState(ELegIdleState::NeedsReturn);
	}
	else
		SetIdleState(ELegIdleState::ArticulatedReturn);
}

void FDragonIdleLegDriver::SetIdleState(const ELegIdleState NewState, const bool SkipBroadcast)
{
	if (NewState == ELegIdleState::Planted || NewState == ELegIdleState::NeedsReturn)
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
	if (!SkipBroadcast)
	{
		OnIdleStateChanged.Broadcast(IdleState, NewState);
	}
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
		if (OldState == ELegIdleState::ArticulatedReturn && RightLegDriver->IdleState == ELegIdleState::NeedsReturn)
		{
			RightLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
		}
	});
	RightLegDriver->OnIdleStateChanged.AddLambda([this](const ELegIdleState OldState, const ELegIdleState)
	{
		if (OldState == ELegIdleState::ArticulatedReturn && LeftLegDriver->IdleState == ELegIdleState::NeedsReturn)
		{
			LeftLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
		}
	});
}

void FDragonIdlePose::Tick(const float DeltaTime)
{
	FProceduralPose::Tick(DeltaTime);

	const auto LeftLeg = LeftLegDriver->GetLeg();
	const auto RightLeg = RightLegDriver->GetLeg();
	
	const auto ShouldLeftDisconnect = LeftLeg->Position.Size() > 25.0f || FUtils::GetRotatorDistance(LeftLeg->Rotation) > 15.0f;
	if (RightLegDriver->IdleState != ELegIdleState::ArticulatedReturn && LeftLegDriver->IdleState == ELegIdleState::Planted && ShouldLeftDisconnect)
	{
		LeftLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
	}

	const auto ShouldRightDisconnect = RightLeg->Position.Size() > 25.0f || FUtils::GetRotatorDistance(RightLeg->Rotation) > 15.0f;
	if (LeftLegDriver->IdleState != ELegIdleState::ArticulatedReturn && RightLegDriver->IdleState == ELegIdleState::Planted && ShouldRightDisconnect)
	{
		RightLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
	}
}

void FDragonIdlePose::SyncStateFrom(const FDragonWalkPose* TargetPose) const
{
	LeftLegDriver->SyncIdleStateFrom(TargetPose->LeftLegDriver);
	RightLegDriver->SyncIdleStateFrom(TargetPose->RightLegDriver);
}
