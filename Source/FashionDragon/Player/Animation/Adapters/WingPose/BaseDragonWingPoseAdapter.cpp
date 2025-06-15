#include "BaseDragonWingPoseAdapter.h"

#include "FashionDragon/DebugTools/QuickDebug.h"

FTransform FBaseDragonWingPoseAdapter::ProcessBone(const FName& BoneName, const FPoseWingEffector& Effector)
{
	auto T = FTransform();
	T.AddToTranslation(FVector(0, 0, 0)); // Placeholder for actual transformation logic
	return T;
}
