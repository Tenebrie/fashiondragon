#include "DragonIdlePose.h"

#include "Drivers/DragonIdleBodyDriver.h"
#include "Drivers/DragonIdleHipsDriver.h"
#include "Drivers/DragonIdleLegDriver.h"
#include "Drivers/DragonIdleWingDriver.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"
#include "FashionDragon/Player/Animation/Poses/DragonTrot/DragonTrotPose.h"
#include "FashionDragon/Player/Animation/Poses/DragonSprint/DragonSprintPose.h"
#include "FashionDragon/Utils/Utils.h"

enum class ELegIdleState;
/**
 * @brief Constructor
 */
FDragonIdlePose::FDragonIdlePose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	RootDriver = new FDragonIdleBodyDriver(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::Primary));
	HipsDriver = new FDragonIdleHipsDriver(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Primary));
	BoneDrivers = { RootDriver, HipsDriver };

	LeftLegDriver = new FDragonIdleLegDriver(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Primary));
	RightLegDriver = new FDragonIdleLegDriver(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
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

	LeftWingDriver = new FDragonIdleWingDriver(Anim, Anim->LeftWing.GetBone(EDriverLayer::Primary));
	RightWingDriver = new FDragonIdleWingDriver(Anim, Anim->RightWing.GetBone(EDriverLayer::Primary));
	WingDrivers = {
		LeftWingDriver,
		RightWingDriver,
	};
}

/**
 * @brief Update
 */
void FDragonIdlePose::Tick(const float DeltaTime)
{
	FProceduralPose::Tick(DeltaTime);

	const auto LeftLeg = LeftLegDriver->GetLeg();
	const auto RightLeg = RightLegDriver->GetLeg();

	const auto IsLeftFar = LeftLeg->GetFlatPosition().Size() > 30.0f || FUtils::GetRotatorDistance(LeftLeg->Rotation) > 20.0f;
	const auto IsRightFar = RightLeg->GetFlatPosition().Size() > 30 || FUtils::GetRotatorDistance(RightLeg->Rotation) > 20.0f;
	const auto IsLeftKindaFar = LeftLeg->GetFlatPosition().Size() > 25.0f || FUtils::GetRotatorDistance(LeftLeg->Rotation) > 15.0f;
	const auto IsRightKindaFar = RightLeg->GetFlatPosition().Size() > 25.0f || FUtils::GetRotatorDistance(RightLeg->Rotation) > 15.0f;
	const auto CanLeftDisconnect = RightLegDriver->IdleState != ELegIdleState::ArticulatedReturn && LeftLegDriver->IdleState == ELegIdleState::Planted;
	const auto CanRightDisconnect = LeftLegDriver->IdleState != ELegIdleState::ArticulatedReturn && RightLegDriver->IdleState == ELegIdleState::Planted;
	if (CanLeftDisconnect && CanRightDisconnect && ((IsLeftFar && IsRightKindaFar) || (IsRightFar && IsLeftKindaFar)))
	{
		if (LeftLeg->Rotation.Yaw > 0.0f)
			LeftLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
		else
			RightLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
	}
	else if (IsLeftFar && CanLeftDisconnect)
	{
		LeftLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
	}
	else if (IsRightFar && CanRightDisconnect)
	{
		RightLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
	}

	auto LeftLegRotation = LeftLegDriver->GetDesiredRotation();
	auto RightLegRotation = RightLegDriver->GetDesiredRotation();
	LeftLegRotation.Yaw = FMath::Clamp(LeftLegRotation.Yaw, -50.0f, 50.0f);
	RightLegRotation.Yaw = FMath::Clamp(RightLegRotation.Yaw, -50.0f, 50.0f);
	LeftLegDriver->SetDesiredRotation(LeftLegRotation);
	RightLegDriver->SetDesiredRotation(RightLegRotation);
}

template<typename DriverT>
void FDragonIdlePose::SyncStateFrom(const DriverT* TargetPose) const
{
	RootDriver->ResetState();
	HipsDriver->ResetState();
	LeftLegDriver->SyncIdleStateFrom(TargetPose->LeftLegDriver);
	RightLegDriver->SyncIdleStateFrom(TargetPose->RightLegDriver);
}

template void FDragonIdlePose::SyncStateFrom(const FDragonWalkPose*) const;
template void FDragonIdlePose::SyncStateFrom(const FDragonTrotPose*) const;
template void FDragonIdlePose::SyncStateFrom(const FDragonSprintPose*) const;

void FDragonIdlePose::ResetState()
{
	RootDriver->ResetState();
	HipsDriver->ResetState();

	LeftLegDriver->LockToWorldGround();
	LeftLegDriver->SetIdleState(ELegIdleState::NeedsReturn, true);
	
	RightLegDriver->LockToWorldGround();
	RightLegDriver->SetIdleState(ELegIdleState::NeedsReturn, true);
	
	if (LeftLegDriver->IdleState == ELegIdleState::NeedsReturn && RightLegDriver->IdleState == ELegIdleState::NeedsReturn)
	{
		const auto FurthestLeg = LeftLegDriver->GetLeg()->Position.Size() > RightLegDriver->GetLeg()->Position.Size() ? LeftLegDriver : RightLegDriver;
		FurthestLeg->SetIdleState(ELegIdleState::ArticulatedReturn);
	}
	else if (LeftLegDriver->IdleState == ELegIdleState::NeedsReturn)
		LeftLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
	else if (RightLegDriver->IdleState == ELegIdleState::NeedsReturn)
		RightLegDriver->SetIdleState(ELegIdleState::ArticulatedReturn);
}
