#include "DragonTrotPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

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
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Stepping:
		LockRealWorldPosition();
		SetWalkingState(ELegWalkingState::Planted);
		break;
	}
}

// std::pair<FVector, FRotator> FDragonTrotLegDriver::GetTargetPosition() const
// {
// 	const std::map<ELegWalkingState, std::pair<FVector, FRotator>> LegStateToPosition =
// 	{
// 		{ ELegWalkingState::Relaxed, LEG_POSE((0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
// 		{ ELegWalkingState::Raised, LEG_POSE((0.0f, 0.0f, 150.0f), (0.0f, 0.0f, 60.0f)) },
// 		{ ELegWalkingState::Planted, LEG_POSE((0.0f, -350.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
// 		{ ELegWalkingState::Stepping, LEG_POSE((0.0f, 550.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
// 		{ ELegWalkingState::Inertia, LEG_POSE((0.0f, -450.0f, 200.0f), (0.0f, 0.0f, 60.0f)) },
// 	};
//
// 	return LegStateToPosition.at(WalkingState);
// }

FDragonTrotPose::FDragonTrotPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BodyDriver = new FDragonWalkBodyDriver(AnimInstance, Anim->ControlledBody, Anim->BackLeftLeg, Anim->BackRightLeg);
	HipsDriver = new FDragonWalkHipSwayDriver(Anim, Anim->ControlledHips, Anim->BackLeftLeg, Anim->BackRightLeg);
	FProceduralPose::BodyDriver = BodyDriver;
	FProceduralPose::HipsDriver = HipsDriver;
	
	LeftLegDriver = new FDragonTrotLegDriver(AnimInstance, Anim->BackLeftLeg);
	RightLegDriver = new FDragonTrotLegDriver(AnimInstance, Anim->BackRightLeg);
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
	LeftLegDriver->LockRealWorldPosition();
	LeftLegDriver->SetWalkingState(ELegWalkingState::Planted);
	RightLegDriver->SetWalkingState(ELegWalkingState::Stepping);
}
