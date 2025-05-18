#include "DragonIdleLegDriver.h"

#include "FashionDragon/Player/Animation/Enums/LegIdleState.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"
#include "FashionDragon/Utils/Utils.h"

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
		.Duration = 0.2f,
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
			.Duration = Duration,
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