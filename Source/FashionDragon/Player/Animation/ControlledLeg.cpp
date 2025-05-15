#include "ControlledLeg.h"

#include "DragonAnimInstance.h"

FControlledLeg::FControlledLeg(UDragonAnimInstance* AnimInstance, const FName IKBoneName, const FVector& IKBoneOffset, const int Idx):
	AnimInstance(AnimInstance), Idx(Idx), IKBoneName(IKBoneName), IKBoneOffset(IKBoneOffset)
{
	
}

FVector FControlledLeg::GetWorldPosition() const
{
	return AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().TransformPosition(Position + IKBoneOffset);
}

FVector FControlledLeg::GetWorldPosition(const FPoseEffector& WithEffector) const
{
	return AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().TransformPosition(WithEffector.Position + IKBoneOffset);
}
