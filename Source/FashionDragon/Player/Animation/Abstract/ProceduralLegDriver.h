#pragma once
#include "ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Limbs/ControlledLeg.h"

class FProceduralLegDriver : public FProceduralBoneDriver
{
protected:
	FControlledLeg* Leg;
	
public:
	FProceduralLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
		: FProceduralBoneDriver(AnimInstance, ControlledLeg), Leg(ControlledLeg) {}
	
	virtual FPoseEffector ToPostProcessEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) { return BaseEffector; }

	FControlledLeg* GetLeg() const { return Leg; }
};

