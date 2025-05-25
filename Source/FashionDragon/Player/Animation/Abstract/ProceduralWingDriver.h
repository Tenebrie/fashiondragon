#pragma once
#include "ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledLeg.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledWing.h"
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

struct FArticulatedValue
{
	float Value = 0.0f;
	float StartArticulation = 0.0f;
	float EndArticulation = 0.0f;

	FArticulatedValue() = default;
	explicit FArticulatedValue(const float Value): Value(Value) {}
	FArticulatedValue(const float Value, const float StartArticulation, const float EndArticulation)
		: Value(Value), StartArticulation(StartArticulation), EndArticulation(EndArticulation) {}
};

struct FDragonWingStateData
{
	FArticulatedValue Flap = FArticulatedValue();
	FArticulatedValue Openness = FArticulatedValue();

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

	float DesiredFlap = 0.0f;
	float DesiredOpenness = 0.0f;

	float StartedFlapFrom = 0.0f;
	float StartedOpennessFrom = 0.0f;
	
	float CycleDuration = 1.0f;

	virtual FDragonWingStateData GetRawStateData() const;
	void RecalculatePose();
	
public:
	FProceduralWingDriver(UDragonAnimInstance* AnimInstance, FControlledWing* ControlledWing);

	virtual void Tick(float DeltaTime);
	virtual void ResetState();

	virtual FPoseWingEffector ToEffector(const FPoseWingEffector& BaseEffector, const FPoseEffectorContext& Context);

	FControlledWing* GetWing() const { return Wing; }
};