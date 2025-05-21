#include "DragonWingPoseAdapter.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "PhysicsEngine/ConstraintInstance.h"

TPair<FVector, FRotator>* FDragonWingPoseAdapter::GetNeutralPose(const int Idx, const float MirrorValue)
{
	TMap<int, TPair<FVector, FRotator>> NeutralPose = {
		{ 0, { FVector(0.0f, 0.0f, 0.0f), FRotator(45.0f, 0.0f, 0.0f) } },
		{ 1, { FVector(0.0f, 0.0f, 0.0f), FRotator(15.0f, 45.0f, 0.0f) } },
		{ 2, { FVector(0.0f, 0.0f, 0.0f), FRotator(30.0f, -60.0f, 0.0f) } },
		{ 3, { FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -35.0f, 0.0f) } },
		{ 4, { FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -15.0f, 0.0f) } },
		{ 5, { FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -25.0f, 0.0f) } },
		{ 6, { FVector(0.0f, 0.0f, 0.0f), FRotator(-25.0f, 5.0f, 10.0f) } },
	};
	return  NeutralPose.Find(Idx);
}

TPair<FVector, FRotator>* FDragonWingPoseAdapter::ApplyMirror(const TPair<FVector, FRotator>* Pair, const float MirrorValue)
{
	const auto Position = FVector(Pair->Key.X, Pair->Key.Y * MirrorValue, Pair->Key.Z);
	const auto Rotation = FRotator(Pair->Value.Pitch, Pair->Value.Yaw * MirrorValue, Pair->Value.Roll * MirrorValue);
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

	const auto FirstFingerBone = Wing->GetBoneName(3);
	const auto SecondFingerBone = Wing->GetBoneName(4);
	const auto ThirdFingerBone = Wing->GetBoneName(5);
	const auto FourthFingerBone = Wing->GetBoneName(6);

	auto RootPose = GetNeutralPose(0, Wing->MirrorModifier);
	auto ChildPose = GetNeutralPose(1, Wing->MirrorModifier);
	auto ThirdPose = GetNeutralPose(2, Wing->MirrorModifier);
	auto FirstFingerPose = GetNeutralPose(3, Wing->MirrorModifier);
	auto SecondFingerPose = GetNeutralPose(4, Wing->MirrorModifier);
	auto ThirdFingerPose = GetNeutralPose(5, Wing->MirrorModifier);
	auto FourthFingerPose = GetNeutralPose(6, Wing->MirrorModifier);
	
	const auto RootRotation = RootPose->Value + FRotator(45.0f * Effector.Flap, 0.0f, 0.0f);
	RootPose->Value = RootRotation;

	const auto Closeness = 1.0f - Effector.Openness;
	RootPose->Value.Yaw += Closeness * 70.0f;
	RootPose->Value.Roll += Closeness * 45.0f;
	ChildPose->Value.Yaw -= Closeness * 160.0f;
	ThirdPose->Value.Pitch -= Closeness * 150.0f;
	ThirdPose->Value.Yaw += Closeness * 90.0f;
	ThirdPose->Value.Roll += Closeness * 90.0f;
	FirstFingerPose->Value.Yaw += Closeness * 80.0f;
	FirstFingerPose->Value.Pitch -= Closeness * 120.0f;
	SecondFingerPose->Value.Yaw += Closeness * 60.0f;
	SecondFingerPose->Value.Pitch -= Closeness * 80.0f;
	SecondFingerPose->Value.Roll += Closeness * 60.0f;
	ThirdFingerPose->Value.Roll -= Closeness * 60.0f;
	ThirdFingerPose->Value.Pitch -= Closeness * 50.0f;
	ThirdFingerPose->Value.Yaw += Closeness * 70.0f;
	FourthFingerPose->Value.Yaw -= Closeness * 25.0f;

	RootPose = ApplyMirror(RootPose, Wing->MirrorModifier);
	ChildPose = ApplyMirror(ChildPose, Wing->MirrorModifier);
	ThirdPose = ApplyMirror(ThirdPose, Wing->MirrorModifier);
	FirstFingerPose = ApplyMirror(FirstFingerPose, Wing->MirrorModifier);
	SecondFingerPose = ApplyMirror(SecondFingerPose, Wing->MirrorModifier);
	ThirdFingerPose = ApplyMirror(ThirdFingerPose, Wing->MirrorModifier);
	FourthFingerPose = ApplyMirror(FourthFingerPose, Wing->MirrorModifier);

	SetBoneOffset(FName("Spine"), RootBone, RootPose->Key, RootPose->Value);
	SetBoneOffset(RootBone, SecondBone, ChildPose->Key, ChildPose->Value);
	SetBoneOffset(SecondBone, ThirdBone, ThirdPose->Key, ThirdPose->Value);
	SetBoneOffset(SecondBone, FirstFingerBone, FirstFingerPose->Key, FirstFingerPose->Value);
	SetBoneOffset(SecondBone, SecondFingerBone, SecondFingerPose->Key, SecondFingerPose->Value);
	SetBoneOffset(SecondBone, ThirdFingerBone, ThirdFingerPose->Key, ThirdFingerPose->Value);
	SetBoneOffset(RootBone, FourthFingerBone, FourthFingerPose->Key, FourthFingerPose->Value);
}
