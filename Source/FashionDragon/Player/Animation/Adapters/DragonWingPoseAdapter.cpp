#include "FashionDragon/Player/Animation/Adapters/DragonWingPoseAdapter.h"

void FDragonWingPoseAdapter::ProcessDrivenBone(FTransform& OutTransform, const FString& BoneName, const FPoseWingEffector& Effector)
{
	const float Closeness = 1.0f - Effector.Openness;
	if (BoneName == "Wing_001_L")
	{
		FRotator NewRotator = OutTransform.Rotator();
		NewRotator.Add(90.0f * Closeness, 0.0f, 0.0f);
		OutTransform.SetRotation(FQuat(NewRotator));
	}
}
