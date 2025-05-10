#include "DragonTrotPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonTrotLegDriver::FDragonTrotLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FAbstractProceduralLegDriver(AnimInstance, ControlledLeg)
{}

// ============================================================================
// Leg Driver
// ============================================================================
void FDragonTrotLegDriver::Tick(const float DeltaTime)
{
	ArticulationPosition = FVector(0.f, 0.f, 0.f);
	// If we're stepping forward, add some vertical offset
	if (WalkingState == ELegWalkingState::Stepping)
		ArticulationPosition.Z = 100.f;
	
	// Advance time forward. Adjusted by character's movement speed.
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;

	FAbstractProceduralLegDriver::Tick(AdvanceValue);
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
		SetWalkingState(ELegWalkingState::Planted);
		break;
	}
}

std::pair<FVector, FRotator> FDragonTrotLegDriver::GetTargetPosition() const
{
	const std::map<ELegWalkingState, std::pair<FVector, FRotator>> LegStateToPosition =
	{
		{ ELegWalkingState::Relaxed, LEG_POSE((0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
		{ ELegWalkingState::Raised, LEG_POSE((0.0f, 0.0f, 150.0f), (0.0f, 0.0f, 60.0f)) },
		{ ELegWalkingState::Planted, LEG_POSE((0.0f, -350.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
		{ ELegWalkingState::Stepping, LEG_POSE((0.0f, 800.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
		{ ELegWalkingState::Inertia, LEG_POSE((0.0f, -350.0f, 200.0f), (0.0f, 0.0f, 60.0f)) },
	};

	return LegStateToPosition.at(WalkingState);
}

FDragonTrotPose::FDragonTrotPose(UDragonAnimInstance* AnimInstance, FControlledLeg* LeftLeg, FControlledLeg* RightLeg): FAbstractProceduralPose(AnimInstance)
{
	const auto LeftDriver = new FDragonTrotLegDriver(AnimInstance, LeftLeg);
	const auto RightDriver = new FDragonTrotLegDriver(AnimInstance, RightLeg);
	LegDrivers = {
		LeftDriver,
		RightDriver,
	};
	LeftDriver->SetWalkingState(Planted);
}
