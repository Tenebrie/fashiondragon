#include "ControlledBone.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FVector FControlledBone::GetWorldPosition(const FVector& FromPosition) const
{
	const auto ComponentTransform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform();
	return ComponentTransform.TransformPosition(FromPosition + BoneOffset);
}

FQuat FControlledBone::GetWorldRotation(const FQuat& FromRotation) const
{
	const auto ComponentTransform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform();
	return ComponentTransform.TransformRotation(FromRotation);
}
