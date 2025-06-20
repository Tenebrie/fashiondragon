#pragma once
#include "ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledWing.h"
#include "FashionDragon/Player/Animation/Structs/ArticulatedValue.h"
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

struct FDragonWingStateData
{
	FArticulatedValue FlapAngle = FArticulatedValue();
	FArticulatedValue TiltAngle = FArticulatedValue();
	FArticulatedValue FlightFoldState = FArticulatedValue();
	FArticulatedValue RestFoldState = FArticulatedValue();

	float Duration = 1.0f;
	float TransitionSpeed = 1.0f;
};

/**
 * @brief Abstract base class for procedural wing drivers
 */
class FProceduralWingDriver: public FBaseDriver
{
protected:
	FControlledWing* Wing;

	FPoseWingEffector DesiredState;
	FPoseWingEffector StartingState;

	FProceduralWingDriverDebugReporter DebugReporter;
	
	virtual FDragonWingStateData GetRawStateData() const;
	void RecalculatePose();
	
public:
	FProceduralWingDriver(UDragonAnimInstance* AnimInstance, FControlledWing* ControlledWing):
		FBaseDriver(AnimInstance), Wing(ControlledWing) {}

	virtual void Tick(float DeltaTime) override;
	virtual void ResetState();

	virtual FPoseWingEffector ToEffector(const FPoseWingEffector& BaseEffector, const FPoseEffectorContext& Context);

	FProceduralWingDriverDebugReporter* GetDebugReporter() { return &DebugReporter; }

	FControlledWing* GetWing() const { return Wing; }
};