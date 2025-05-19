#include "DragonIdlePose.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Utils/Utils.h"

/**
 * @brief Constructor
 */
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

	LeftWingDriver = new FDragonIdleWingDriver(Anim, Anim->LeftWing);
	RightWingDriver = new FDragonIdleWingDriver(Anim, Anim->RightWing);
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

/**
 * @brief Sync state from walk pose on transition
 * TODO: Check if necessary anymore?
 */
void FDragonIdlePose::SyncStateFrom(const FDragonWalkPose* TargetPose) const
{
	BodyDriver->ResetState();
	LeftLegDriver->SyncIdleStateFrom(TargetPose->LeftLegDriver);
	RightLegDriver->SyncIdleStateFrom(TargetPose->RightLegDriver);
}
