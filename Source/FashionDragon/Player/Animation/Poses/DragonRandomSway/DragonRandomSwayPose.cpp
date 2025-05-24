#include "DragonRandomSwayPose.h"

#include "Drivers/DragonRandomSwayDriverBody.h"
#include "Drivers/DragonRandomSwayDriverHips.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonRandomSwayPose::FDragonRandomSwayPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BodyDrivers = { new FDragonRandomSwayDriverBody(Anim, Anim->ControlledBody.GetBone(EDriverLayer::Sway)) };
	HipsDrivers = { new FDragonRandomSwayDriverHips(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Sway)) };
}
