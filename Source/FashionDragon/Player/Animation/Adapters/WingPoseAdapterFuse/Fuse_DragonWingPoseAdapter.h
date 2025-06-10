#pragma once

struct FPoseWingEffector;

class FASHIONDRAGON_API FFuse_DragonWingPoseAdapter final
{
public:
	static void Evaluate(FPoseContext& Output, const FPoseWingEffector& LeftWingEffector, const FPoseWingEffector& RightWingEffector);
};
