#include "DragonIdlePose.h"

// ============================================================================
// Leg Driver
// ============================================================================

FDragonIdleLegDriver::FDragonIdleLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FAbstractProceduralLegDriver(AnimInstance, ControlledLeg)
{}

std::pair<FVector, FRotator> FDragonIdleLegDriver::GetTargetPosition() const
{
	return LEG_POSE((0.f, 0.f, 0.f), (0.f, 0.f, 0.f));
}

FDragonIdlePose::FDragonIdlePose(UDragonAnimInstance* AnimInstance, FControlledLeg* LeftLeg, FControlledLeg* RightLeg): FAbstractProceduralPose(AnimInstance)
{
	LegDrivers = {
		new FDragonIdleLegDriver(AnimInstance, LeftLeg),
		new FDragonIdleLegDriver(AnimInstance, RightLeg)
	};
}
