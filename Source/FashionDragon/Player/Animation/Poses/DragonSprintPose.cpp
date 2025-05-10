#include "DragonSprintPose.h"

#include <map>

FDragonSprintLegDriver::FDragonSprintLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FAbstractProceduralLegDriver(AnimInstance, ControlledLeg)
{}

// ============================================================================
// Leg Driver
// ============================================================================
std::pair<FVector, FRotator> FDragonSprintLegDriver::GetTargetPosition() const
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

FDragonSprintPose::FDragonSprintPose(UDragonAnimInstance* AnimInstance, FControlledLeg* LeftLeg,
	FControlledLeg* RightLeg): FAbstractProceduralPose(AnimInstance)
{
	const auto LeftDriver = new FDragonSprintLegDriver(AnimInstance, LeftLeg);
	const auto RightDriver = new FDragonSprintLegDriver(AnimInstance, RightLeg);
	LegDrivers = {
		LeftDriver,
		RightDriver,
	};
	LeftDriver->SetWalkingState(Planted);
}
