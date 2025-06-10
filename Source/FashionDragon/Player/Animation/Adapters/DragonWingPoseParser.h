#pragma once
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

class FDragonWingPoseParser
{
public:
	static void Evaluate(FPoseContext& Output, const FPoseWingEffector& LeftWingEffector, const FPoseWingEffector& RightWingEffector);
	static void ProcessBoneAsDriven(FPoseContext& Output, const FString& BoneName, const FPoseWingEffector& Effector);
	static void ProcessBoneAsHelper(FPoseContext& Output, const FString& BoneName, const TArray<FString>& Parents);
	static void ProcessHelperBone(FTransform& Bone, const FTransform& RestOffset, const TArray<FTransform>& Parents, const TArray<FTransform>& ParentsRest);
};
