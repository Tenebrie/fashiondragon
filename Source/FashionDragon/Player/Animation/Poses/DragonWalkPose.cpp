#include "DragonWalkPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

// ============================================================================
// Body Driver
// ============================================================================

void FDragonWalkBodyDriver::Tick(float DeltaTime)
{
	const auto LeftLegOffset = std::min(1.0, LeftLeg->Position.Size() / 750.0f);
	const auto RightLegOffset = std::min(1.0, RightLeg->Position.Size() / 750.0f);

	const auto LegState = std::min(LeftLegOffset, RightLegOffset);
	const auto VerticalOffset = LegState * -50.0f;

	Position = FVector(0.0f, 0.0f, VerticalOffset);


	// Lerp current value to target value
	auto TargetLean = 10.0f + (LegState - 0.5f) * 5.f;

	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (OwningActor->IsSprinting)
	{
		TargetLean += 10.0f;
	}

	const auto Lean = FMath::Lerp(Bone->Rotation.Roll, TargetLean, DeltaTime);
	
	Rotation = FRotator(0.0f, 0.0f, Lean);
}

// ============================================================================
// Hip Sway Driver
// ============================================================================

void FDragonWalkHipSwayDriver::Tick(const float DeltaTime)
{
	FAbstractProceduralDriver::Tick(DeltaTime);

	// TODO: Calculate the sway value based on the position of each leg.
	// Specifically, a leg that's in Planted state contributes negative weight, while a leg in Stepping state is positive.
	const auto LeftLegOffset = std::min(1.0, LeftLeg->Position.Size() / 750.0f);
	const auto RightLegOffset = std::min(1.0, RightLeg->Position.Size() / 750.0f);

	const auto Sway = FMath::Sin(CyclePosition * 2.0f * PI) * 10.0f;
	
	Rotation = FRotator(0.0f, Sway, 0.0f);
}

// ============================================================================
// Leg Driver
// ============================================================================

FDragonWalkLegDriver::FDragonWalkLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FAbstractProceduralLegDriver(AnimInstance, ControlledLeg)
{}

void FDragonWalkLegDriver::Tick(const float DeltaTime)
{
	// If we're stepping forward, add some vertical offset
	if (WalkingState == ELegWalkingState::Stepping)
	{
		ArticulationPosition.Z = 100.f;
		ArticulationRotation.Z = 50.0f;
	}
	
	// Advance time forward. Adjusted by character's movement speed.
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;
	
	FAbstractProceduralLegDriver::Tick(AdvanceValue);

	// If the leg is stretched too far, disconnect
	if (WalkingState == ELegWalkingState::Planted && Leg->Position.Size() > 300.0f && Leg->Position.Y < 0.0f)
	{
		SetWalkingState(ELegWalkingState::Inertia, true);
	}
}

void FDragonWalkLegDriver::AdvanceState()
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
		LockTargetWorldPosition();
		SetWalkingState(ELegWalkingState::Planted);
		break;
	}
}

std::pair<FVector, FRotator> FDragonWalkLegDriver::GetTargetPosition() const
{
	const std::map<ELegWalkingState, std::pair<FVector, FRotator>> LegStateToPosition =
	{
		{ ELegWalkingState::Relaxed, LEG_POSE((0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
		{ ELegWalkingState::Raised, LEG_POSE((0.0f, 0.0f, 150.0f), (0.0f, 0.0f, 60.0f)) },
		{ ELegWalkingState::Planted, LEG_POSE((0.0f, -350.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
		{ ELegWalkingState::Stepping, LEG_POSE((0.0f, 500.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
		{ ELegWalkingState::Inertia, LEG_POSE((0.0f, -350.0f, 200.0f), (0.0f, 0.0f, 60.0f)) },
	};
	
	return LegStateToPosition.at(WalkingState);
}

FDragonWalkPose::FDragonWalkPose(UDragonAnimInstance* Anim): FAbstractProceduralPose(Anim)
{
	BodyDriver = new FDragonWalkBodyDriver(Anim, Anim->ControlledBody, Anim->BackLeftLeg, Anim->BackRightLeg);
	HipsDriver = new FDragonWalkHipSwayDriver(Anim, Anim->ControlledHips, Anim->BackLeftLeg, Anim->BackRightLeg);
	FAbstractProceduralPose::BodyDriver = BodyDriver;
	FAbstractProceduralPose::HipsDriver = HipsDriver;

	LeftLegDriver = new FDragonWalkLegDriver(Anim, Anim->BackLeftLeg);
	RightLegDriver = new FDragonWalkLegDriver(Anim, Anim->BackRightLeg);
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};
}

void FDragonWalkPose::SyncStateFrom(const FDragonTrotPose* TargetPose) const
{
	LeftLegDriver->SyncStateFrom(TargetPose->LeftLegDriver);
	RightLegDriver->SyncStateFrom(TargetPose->RightLegDriver);
}

void FDragonWalkPose::ResetState()
{
	LeftLegDriver->LockRealWorldPosition();
	LeftLegDriver->SetWalkingState(Planted);
	RightLegDriver->SetWalkingState(Stepping);
}
