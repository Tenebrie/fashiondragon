#include "DragonIdlePose.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

// ============================================================================
// Leg Driver
// ============================================================================

FDragonIdleLegDriver::FDragonIdleLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FAbstractProceduralLegDriver(AnimInstance, ControlledLeg)
{}

std::pair<FVector, FRotator> FDragonIdleLegDriver::GetTargetPosition() const
{
	return LEG_POSE((0.f, 0.f, 0.f), (0.f, 0.f, 0.f));
}

FDragonIdlePose::FDragonIdlePose(UDragonAnimInstance* Anim): FAbstractProceduralPose(Anim)
{
	BodyDriver = new FDragonIdleBodyDriver(Anim, Anim->ControlledBody);
	LegDrivers = {
		new FDragonIdleLegDriver(Anim, Anim->BackLeftLeg),
		new FDragonIdleLegDriver(Anim, Anim->BackRightLeg)
	};
}
