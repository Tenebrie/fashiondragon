#pragma once
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

class FDragonWingPoseAdapter
{
public:
	static void ProcessDrivenBone(FTransform& OutTransform, const FString& BoneName, const FPoseWingEffector& Effector);
};
