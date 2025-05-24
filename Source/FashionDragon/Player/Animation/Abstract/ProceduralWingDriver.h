#pragma once
#include "ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledLeg.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledWing.h"
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

struct FDragonWingStateData
{
	float Flap = 0.0f;
	float Openness = 0.0f;

	/**
	 * Duration of the state in seconds.
	 * Should not be modified dynamically.
	 */
	float Duration = 1.0f;
	/**
	 * Playback speed. Can be safely modified dynamically.
	 */
	mutable float PlaybackSpeed = 1.0f;
	
	float TransitionSpeed = 1.0f;

	FVector StartArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
	FVector EndArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector EndArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
};

/**
 * @brief Abstract base class for procedural wing drivers
 */
class FProceduralWingDriver: public FBaseDriver
{
protected:
	FControlledWing* Wing;

	float CycleDuration = 1.0f;
	bool Inverted = false;

	// virtual void AdvanceState();
	virtual FDragonWingStateData GetTargetPosition() const;
	
public:
	FProceduralWingDriver(UDragonAnimInstance* AnimInstance, FControlledWing* ControlledWing);

	virtual void Tick(float DeltaTime);
	virtual void ResetState();

	virtual FPoseWingEffector ToEffector(const FPoseWingEffector& BaseEffector, const FPoseEffectorContext& Context);

	FControlledWing* GetWing() const { return Wing; }
};