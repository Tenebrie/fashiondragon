#pragma once
#include "AbstractProceduralLegDriver.h"

class FAbstractProceduralPose
{
protected:
	UDragonAnimInstance* AnimInstance;

	TArray<FAbstractProceduralLegDriver*> LegDrivers;
	
public:
	virtual ~FAbstractProceduralPose() = default;

	explicit FAbstractProceduralPose(UDragonAnimInstance* AnimInstance);

	float BlendAlpha = 0.0f;
	virtual void Tick(float DeltaTime);
	virtual void ResetState() {};
	FPoseEffector ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg) const; 
};
