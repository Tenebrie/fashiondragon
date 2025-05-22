#include "DragonTrotPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"

FDragonTrotLegDriver::FDragonTrotLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FProceduralLegDriver(AnimInstance, ControlledLeg)
{}

// ============================================================================
// Leg Driver
// ============================================================================
void FDragonTrotLegDriver::Tick(const float DeltaTime)
{
	// Advance time forward. Adjusted by character's movement speed.
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;

	FProceduralLegDriver::Tick(AdvanceValue);

	// If the leg is stretched too far, disconnect
	if (WalkingState == ELegWalkingState::Planted && Leg->Position.Size() > 350.0f && Leg->Position.Y < 0.0f)
	{
		SetWalkingState(ELegWalkingState::Inertia, true);
	}
}

void FDragonTrotLegDriver::AdvanceState()
{
	switch (WalkingState)
	{
	case ELegWalkingState::Relaxed:
		SetWalkingState(ELegWalkingState::Raised);
		break;
	case ELegWalkingState::Raised:
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Inertia:
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Planted:
	case ELegWalkingState::SeekingGround:
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Stepping:
		LockToWorldGround();
		break;
	}
}

FDragonTrotPose::FDragonTrotPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BodyDriver = new FDragonWalkBodyDriver(Anim, Anim->ControlledBody.GetBone(EBodyDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EBodyDriverLayer::Primary), Anim->BackRightLeg.GetBone(EBodyDriverLayer::Primary));
	HipsDriver = new FDragonWalkHipSwayDriver(Anim, Anim->ControlledHips.GetBone(EBodyDriverLayer::Primary),  Anim->BackLeftLeg.GetBone(EBodyDriverLayer::Primary), Anim->BackRightLeg.GetBone(EBodyDriverLayer::Primary));
	BodyDrivers = { BodyDriver };
	HipsDrivers = { HipsDriver };
	
	LeftLegDriver = new FDragonTrotLegDriver(Anim, Anim->BackLeftLeg.GetBone(EBodyDriverLayer::Primary));
	RightLegDriver = new FDragonTrotLegDriver(Anim, Anim->BackRightLeg.GetBone(EBodyDriverLayer::Primary));
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};
	LeftLegDriver->SetWalkingState(ELegWalkingState::Planted);
}

void FDragonTrotPose::SyncStateFrom(const FDragonWalkPose* TargetPose) const
{
	BodyDriver->SyncStateFrom(TargetPose->BodyDriver);
	HipsDriver->SyncStateFrom(TargetPose->HipsDriver);
	LeftLegDriver->SyncStateFrom(TargetPose->LeftLegDriver);
	RightLegDriver->SyncStateFrom(TargetPose->RightLegDriver);
}

void FDragonTrotPose::ResetState()
{
	BodyDriver->ResetState();
	HipsDriver->ResetState();
	LeftLegDriver->LockToWorldGround();
	RightLegDriver->SetWalkingState(ELegWalkingState::Stepping);
}
