#pragma once
#include "AbstractProceduralDriver.h"
#include "AbstractProceduralLegDriver.h"

class FAbstractProceduralDriver;

class FAbstractProceduralPose
{
protected:
	UDragonAnimInstance* AnimInstance;

	FAbstractProceduralDriver* BodyDriver = nullptr;
	FAbstractProceduralDriver* HipsDriver = nullptr;
	TArray<FAbstractProceduralLegDriver*> LegDrivers;
	
public:
	virtual ~FAbstractProceduralPose() = default;

	explicit FAbstractProceduralPose(UDragonAnimInstance* AnimInstance);

	float BlendAlpha = 0.0f;
	virtual void Tick(float DeltaTime);
	virtual void ResetState() {};
	FPoseEffector ToBodyEffector(const FPoseEffector& BaseEffector, const FControlledBone* Body) const;
	FPoseEffector ToHipsEffector(const FPoseEffector& BaseEffector, const FControlledBone* Hips) const;
	FPoseEffector ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg) const; 
};
