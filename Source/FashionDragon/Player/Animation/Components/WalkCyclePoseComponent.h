#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPoseComponent.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledLeg.h"

class FProceduralPose;
class FProceduralLegSteppingDriver;

using FBreakpoint=TPair<float, ELegWalkingState>;

class FWalkCyclePoseComponent final : public FProceduralPoseComponent
{
	FProceduralLegSteppingDriver* LeftLegDriver;
	FProceduralLegSteppingDriver* RightLegDriver;

	float TimeSinceLastSync = 0.0f;
	bool SwitchStartingLeg = false;
	int LeftState = -1;
	int RightState = -1;
	
	float LeftCyclePosition = 0.0f;
	float RightCyclePosition = 0.0f;
	TArray<FBreakpoint> CycleBreakpoints;
	
	float CycleDuration = 1.0f;
	
public:
	FWalkCyclePoseComponent(FProceduralPose* Pose, FProceduralLegSteppingDriver* LeftLegDriver, FProceduralLegSteppingDriver* RightLegDriver)
		: FProceduralPoseComponent(Pose), LeftLegDriver(LeftLegDriver), RightLegDriver(RightLegDriver) {}

	void SetCycleBreakpoints(const TArray<FBreakpoint>& Breakpoints);
	void Mirror();
	int CheckForBreakpoint(const float Position, const int StateIndex, FProceduralLegSteppingDriver* Driver);
	void SyncStateFrom(const FWalkCyclePoseComponent* Other);
	void ResetState();
	
	virtual void Tick(const float DeltaTime) override;
};
