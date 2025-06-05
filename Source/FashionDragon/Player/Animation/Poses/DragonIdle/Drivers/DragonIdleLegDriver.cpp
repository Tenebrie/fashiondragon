#include "DragonIdleLegDriver.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Enums/LegIdleState.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"
#include "FashionDragon/Player/Animation/Poses/DragonTrot/DragonTrotPose.h"
#include "FashionDragon/Player/Animation/Poses/DragonSprint/DragonSprintPose.h"
#include "FashionDragon/Utils/Utils.h"

FDragonWalkStateData FDragonIdleLegDriver::GetRawWalkStateData() const
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

void FDragonIdleLegDriver::Tick(const float DeltaTime)
{
	FProceduralLegSteppingDriver::Tick(DeltaTime);

	if (BlendAlpha <= 0.0f)
		return;

	if (IdleState == ELegIdleState::ArticulatedReturn && CyclePosition >= GetRawWalkStateData().Duration)
	{
		LockToWorldGround();
		SetIdleState(ELegIdleState::Planted);
	}

	const auto LegReference = RotateVectorToInputRotation(Leg->Position, true);
	const auto ShouldDisconnect = FMath::Abs(LegReference.X) > 200.f || LegReference.Z < -150.0f || LegReference.Y > 600.0f || LegReference.Y < -200.0f
		|| FUtils::GetRotatorDistance(Leg->Rotation) > 50.0f;
	if (IdleState == ELegIdleState::Planted && ShouldDisconnect)
	{
		SetIdleState(ELegIdleState::ArticulatedReturn);
	}
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
			.TargetPosition = FVector::ZeroVector,
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

FString FDragonIdleLegDriver::GetDebugState() const
{
	return FProceduralLegSteppingDriver::GetDebugState().Append(", ").Append(FUtils::EnumToString(IdleState));
}

template<typename DriverT>
void FDragonIdleLegDriver::SyncIdleStateFrom(const DriverT* TargetDriver)
{
	ResetState();
	if (TargetDriver->WalkingState == ELegWalkingState::Planted)
	{
		LockToWorldGround();
		SetIdleState(ELegIdleState::NeedsReturn, true);
	}
	else
		SetIdleState(ELegIdleState::ArticulatedReturn, true);
}

template void FDragonIdleLegDriver::SyncIdleStateFrom(const FDragonWalkLegDriver*);
template void FDragonIdleLegDriver::SyncIdleStateFrom(const FDragonTrotLegDriver*);
template void FDragonIdleLegDriver::SyncIdleStateFrom(const FDragonSprintLegDriver*);