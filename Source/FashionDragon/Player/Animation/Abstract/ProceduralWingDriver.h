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
	 * Maximum force an effector may apply to the bone in this state.
	 * Effectively, maximum speed per second.
	 */
	float LinearForce = 10000.0f;
	/**
	 * Maximum force an effector may apply to the bone's rotation in this state.
	 * Effectively, maximum speed (angles) per second.
	 */
	float AngularForce = 360.0f;
	/**
	 * Duration of the state in seconds.
	 * Should not be modified dynamically.
	 */
	float Duration = 1.0f;

	/**
	 * Playback speed. Can be safely modified dynamically.
	 */
	mutable float PlaybackSpeed = 1.0f;

	FVector StartArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
	FVector EndArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector EndArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
};

/**
 * @brief Abstract base class for procedural wing drivers
 */
class FProceduralWingDriver
{
protected:
	UDragonAnimInstance* AnimInstance;
	FControlledWing* Wing;
	float CyclePosition = 0.0f;
	float VisualCyclePosition = 0.0f;

	float CycleDuration = 1.0f;
	bool Inverted = false;

	// virtual void AdvanceState();
	virtual FDragonWingStateData GetTargetPosition() const;
	
public:
	virtual ~FProceduralWingDriver() = default;
	FProceduralWingDriver(UDragonAnimInstance* AnimInstance, FControlledWing* ControlledWing);

	virtual void Tick(float DeltaTime);
	// virtual void RecalculatePose(const float DeltaTime);

	virtual FPoseWingEffector ToEffector(const FPoseWingEffector& BaseEffector, const FPoseEffectorContext& Context);

	FControlledWing* GetWing() const { return Wing; }

	// TODO: Extract into a separate interface
	ELegWalkingState WalkingState = ELegWalkingState::Relaxed;
	// virtual void SetWalkingState(ELegWalkingState NewState, const bool KeepCycle = false);
};