#include "DragonWingPoseAdapter.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "PhysicsEngine/ConstraintInstance.h"

TPair<FVector, FRotator>* FDragonWingPoseAdapter::GetNeutralPose(const int Idx, const float MirrorValue)
{
	TMap<int, TPair<FVector, FRotator>> NeutralPose = {
		{ 0, { FVector(0.0f, 0.0f, 0.0f), FRotator(45.0f, 0.0f, 0.0f) } },
		{ 1, { FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 65.0f, 0.0f) } },
		{ 2, { FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f) } },
	};
	const auto Pair = NeutralPose.Find(Idx);
	const auto Position = FVector(Pair->Key.X, Pair->Key.Y * MirrorValue, Pair->Key.Z);
	const auto Rotation = FRotator(Pair->Value.Pitch, Pair->Value.Yaw * MirrorValue, Pair->Value.Roll);
	return new TPair<FVector, FRotator>(Position, Rotation);
}

void FDragonWingPoseAdapter::SetBoneOffset(
	const FName ParentBone,
	const FName ChildName,
	const FVector& Position,
	const FRotator& Rotation) const
{
	const FRotator FixedRotation = FRotator(Rotation.Yaw, Rotation.Roll, -Rotation.Pitch + 90);
	for (const auto Constraints = Anim->GetSkelMeshComponent()->Constraints; FConstraintInstance* Constraint : Constraints)
	{
		if (Constraint->GetParentBoneName() == ParentBone && Constraint->GetChildBoneName() == ChildName)
		{
			Constraint->SetRefPosition(EConstraintFrame::Frame1, Position);

			FVector PriAxis = FixedRotation.RotateVector(FVector::ForwardVector);
			FVector SecAxis = FixedRotation.RotateVector(FVector::UpVector);
			Constraint->SetRefOrientation(EConstraintFrame::Frame1, PriAxis, SecAxis);
		}
	}
}

void FDragonWingPoseAdapter::ApplyEffector(
	const FControlledWing* Wing,
	const FPoseWingEffector& Effector) const
{
	const auto RootBone = Wing->GetBoneName(0);
	const auto SecondBone = Wing->GetBoneName(1);
	const auto ThirdBone = Wing->GetBoneName(2);

	const auto RootNeutralPose = GetNeutralPose(0, Wing->MirrorModifier);
	const auto ChildNeutralPose = GetNeutralPose(1, Wing->MirrorModifier);
	const auto ThirdNeutralPose = GetNeutralPose(2, Wing->MirrorModifier);

	const auto RootRotation = RootNeutralPose->Value + FRotator(45.0f * Effector.Flap, 0.0f, 0.0f);

	SetBoneOffset(FName("Spine"), RootBone, RootNeutralPose->Key, RootRotation);
	SetBoneOffset(RootBone, SecondBone, ChildNeutralPose->Key, ChildNeutralPose->Value);
	SetBoneOffset(SecondBone, ThirdBone, ThirdNeutralPose->Key, ThirdNeutralPose->Value);
}
