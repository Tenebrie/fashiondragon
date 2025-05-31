#include "DragonRandomSwayPose.h"

#include "DragonRandomSwayDriverBody.h"
#include "DragonRandomSwayDriverHips.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonRandomSwayPose::FDragonRandomSwayPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	RootDrivers = { new FDragonRandomSwayDriverBody(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::Sway)) };
	HipsDrivers = { new FDragonRandomSwayDriverHips(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Sway)) };
}
