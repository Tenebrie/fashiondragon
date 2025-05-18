#pragma once

#include "FashionDragon/Player/Animation/Limbs/ControlledWing.h"
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

class FDragonWingPoseAdapter
{
	UDragonAnimInstance* Anim;
public:
	explicit FDragonWingPoseAdapter(UDragonAnimInstance* Anim)
		: Anim(Anim)
	{
	}

	static TPair<FVector, FRotator>* GetNeutralPose(const int Idx, const float MirrorValue);

	void SetBoneOffset(FName ParentBone, FName ChildName, const FVector& Position, const FRotator& Rotation) const;
	
	void ApplyEffector(
		const FControlledWing* Wing,
		const FPoseWingEffector& Effector
	) const;
};
