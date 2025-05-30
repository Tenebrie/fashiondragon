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
	BodyDriver = new FDragonIdleBodyDriver(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::Primary));
	HipsDriver = new FDragonIdleHipsDriver(Anim, Anim->ControlledTail.GetBone(EDriverLayer::Primary));
	RootDrivers = { BodyDriver };
	TailDrivers = { HipsDriver };

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

template<typename DriverT>
void FDragonIdlePose::SyncStateFrom(const DriverT* TargetPose) const
{
	BodyDriver->ResetState();
	HipsDriver->ResetState();
	LeftLegDriver->SyncIdleStateFrom(TargetPose->LeftLegDriver);
	RightLegDriver->SyncIdleStateFrom(TargetPose->RightLegDriver);
}

template void FDragonIdlePose::SyncStateFrom(const FDragonWalkPose*) const;
template void FDragonIdlePose::SyncStateFrom(const FDragonTrotPose*) const;
template void FDragonIdlePose::SyncStateFrom(const FDragonSprintPose*) const;

void FDragonIdlePose::ResetState()
{
	BodyDriver->ResetState();
	HipsDriver->ResetState();

	LeftLegDriver->LockToWorldGround();
	LeftLegDriver->SetIdleState(ELegIdleState::NeedsReturn);
	RightLegDriver->LockToWorldGround();
	RightLegDriver->SetIdleState(ELegIdleState::NeedsReturn);

	const auto FurthestLeg = LeftLegDriver->GetLeg()->Position.Size() > RightLegDriver->GetLeg()->Position.Size() ? LeftLegDriver : RightLegDriver;
	FurthestLeg->SetIdleState(ELegIdleState::ArticulatedReturn);
}
