#pragma once
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

class FASHIONDRAGON_API FBaseDragonWingPoseAdapter
{
public:
	static FTransform ProcessBone(const FName& BoneName, const FPoseWingEffector& Effector);
};
