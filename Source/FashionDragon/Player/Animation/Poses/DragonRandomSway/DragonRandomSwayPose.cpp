#include "DragonRandomSwayPose.h"

#include "DragonRandomSwayDriverHead.h"
#include "DragonRandomSwayDriverRoot.h"
#include "DragonRandomSwayDriverHips.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonRandomSwayPose::FDragonRandomSwayPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	const auto Root = new FDragonRandomSwayDriverRoot(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::Sway));
	const auto Head = new FDragonRandomSwayDriverHead(Anim, Anim->ControlledHead.GetBone(EDriverLayer::Sway));
	const auto Body = new FDragonRandomSwayDriverRoot(Anim, Anim->ControlledBody.GetBone(EDriverLayer::Sway));
	const auto Hips = new FDragonRandomSwayDriverHips(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Sway));

	Body->SetRotationAmplitude(3, 0.07f);
	BoneDrivers = { Root, Head, Body, Hips };
}
