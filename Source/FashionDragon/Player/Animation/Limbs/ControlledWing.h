#pragma once

class UDragonAnimInstance;

class FControlledWing
{
	UDragonAnimInstance* AnimInstance;
	int Idx;
	
public:
	FControlledWing(UDragonAnimInstance* AnimInstance, const int Idx)
		: AnimInstance(AnimInstance), Idx(Idx)
	{
		if (Idx == 1)
		{
			MirrorModifier = -1.0f;
		}
	}

	float MirrorModifier = 1.0f;
	// Flap state [-1.0f, 1.0f]
	float Flap = 0.0f;
	// 0: Wing folded, 1: Wing fully extended
	float Openness = 0.0f;

	FName GetBoneName(int BoneIdx) const;
};
