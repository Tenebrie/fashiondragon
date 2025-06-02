#include "DragonRandomSwayPose.h"

#include "DragonRandomSwayDriverBody.h"
#include "DragonRandomSwayDriverHips.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonRandomSwayPose::FDragonRandomSwayPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BoneDrivers = {
		new FDragonRandomSwayDriverBody(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::Sway)),
		new FDragonRandomSwayDriverHips(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Sway))
	};
}
