#pragma once
#include "ControlledLeg.h"

#define LEG_POSE(Vector, Rotator) std::make_pair(FVector Vector, FRotator Rotator)

/**
 * @brief Abstract base class for procedural leg drivers
 */
class FAbstractProceduralLegDriver
{
protected:
	UDragonAnimInstance* AnimInstance;
	FControlledLeg* Leg;
	float CyclePosition = 0.0f;
	float VisualCyclePosition = 0.0f;

	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector PositionFrom = FVector(0.0f, 0.0f, 0.0f);
	FRotator RotationFrom = FRotator(0.0f, 0.0f, 0.0f);
	FVector LockedWorldPosition = FVector(0.0f, 0.0f, 0.0f);
	FRotator LockedWorldRotation = FRotator(0.0f, 0.0f, 0.0f);

	FVector ArticulationPosition = FVector(0.0f, 0.0f, 0.0f);
	FVector ArticulationRotation = FVector(0.0f, 0.0f, 0.0f);
	
	virtual void ApplyWorldLock();

	virtual void AdvanceState();
	virtual std::pair<FVector, FRotator> GetTargetPosition() const;
	
public:
	virtual ~FAbstractProceduralLegDriver() = default;

	FAbstractProceduralLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime);

	FControlledLeg* GetLeg() const { return Leg; }
	FVector GetPosition() const { return Position; }
	FRotator GetRotation() const { return Rotation; }

	// TODO: Extract into a separate interface
	ELegWalkingState WalkingState = ELegWalkingState::Relaxed;
	virtual void SetWalkingState(ELegWalkingState NewState, const bool KeepCycle = false);
};