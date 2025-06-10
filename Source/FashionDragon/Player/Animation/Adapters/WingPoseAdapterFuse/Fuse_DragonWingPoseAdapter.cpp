#include "Fuse_DragonWingPoseAdapter.h"

#include "FashionDragon/Player/Animation/Adapters/DragonWingPoseParser.h"

void FFuse_DragonWingPoseAdapter::Evaluate(FPoseContext& Output, const FPoseWingEffector& LeftWingEffector,
                                           const FPoseWingEffector& RightWingEffector)
{
	FDragonWingPoseParser::Evaluate(Output, LeftWingEffector, RightWingEffector);
}
