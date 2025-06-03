#pragma once
#include "FashionDragon/Common/Object.h"

class FProceduralPose;

class FProceduralPoseComponent : public FObject
{
protected:
	FProceduralPose* Pose;
	
public:
	explicit FProceduralPoseComponent(FProceduralPose* Pose): Pose(Pose) {}

	virtual void NativeBeginPlay() {}
	virtual void Tick(float DeltaTime) {}
};
