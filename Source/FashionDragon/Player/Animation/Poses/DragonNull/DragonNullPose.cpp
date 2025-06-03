#include "DragonNullPose.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonNullPose::FDragonNullPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	for (int i = 0; i < static_cast<int>(EDriverLayer::TotalCount); i++)
	{
		BoneDrivers.Add(new FDragonNullDriverBone(AnimInstance, AnimInstance->ControlledRoot.GetBone(static_cast<EDriverLayer>(i))));
		BoneDrivers.Add(new FDragonNullDriverBone(AnimInstance, AnimInstance->ControlledHead.GetBone(static_cast<EDriverLayer>(i))));
		BoneDrivers.Add(new FDragonNullDriverBone(AnimInstance, AnimInstance->ControlledBody.GetBone(static_cast<EDriverLayer>(i))));
		BoneDrivers.Add(new FDragonNullDriverBone(AnimInstance, AnimInstance->ControlledHips.GetBone(static_cast<EDriverLayer>(i))));
		LegDrivers.Add(new FDragonNullDriverLeg(AnimInstance, AnimInstance->BackLeftLeg.GetBone(static_cast<EDriverLayer>(i))));
		LegDrivers.Add(new FDragonNullDriverLeg(AnimInstance, AnimInstance->BackRightLeg.GetBone(static_cast<EDriverLayer>(i))));
	}
}

void FDragonNullPose::EvaluateBlending()
{
	for (FProceduralBoneDriver* NullDriver : BoneDrivers)
	{
		float TotalBoneAlpha = 0.0f;
		for (const FProceduralPose* Pose : AnimInstance->StateMachine->PoseDrivers)
		{
			if (Pose == this) { continue; }

			for (const FProceduralBoneDriver* Driver : Pose->ListBoneDrivers())
			{
				if (Driver->GetBone() == NullDriver->GetBone())
				{
					TotalBoneAlpha += Driver->GetBlendAlpha();
				}
			}
		}

		if (TotalBoneAlpha < 1.0f)
		{
			NullDriver->SetBlendAlpha(1.0f - TotalBoneAlpha);
		}
		else
			NullDriver->SetBlendAlpha(0.0f);
	}

	for (FProceduralLegDriver* NullDriver : LegDrivers)
	{
		float TotalBoneAlpha = 0.0f;
		for (const FProceduralPose* Pose : AnimInstance->StateMachine->PoseDrivers)
		{
			if (Pose == this) { continue; }

			for (const FProceduralLegDriver* Driver : Pose->ListLegDrivers())
			{
				if (Driver->GetLeg() == NullDriver->GetLeg())
				{
					TotalBoneAlpha += Driver->GetBlendAlpha();
				}
			}
		}

		if (TotalBoneAlpha < 1.0f)
		{
			NullDriver->SetBlendAlpha(1.0f - TotalBoneAlpha);
		}
		else
		{
			NullDriver->SetBlendAlpha(0.0f);
		}
	}
}
