#pragma once
#include "FashionDragon/Player/Animation/Enums/DriverLayer.h"
#include "FashionDragon/Player/Animation/Structs/PoseEffector.h"

class UDragonAnimInstance;

class FControlledBone
{
protected:
	UDragonAnimInstance* AnimInstance;
public:
	explicit FControlledBone(UDragonAnimInstance* AnimInstance) : AnimInstance(AnimInstance) {};
	FControlledBone(const FControlledBone& Other) = default;

	bool CanProduceEvents = false;
	EDriverLayer Layer = EDriverLayer::Unknown;
	FName GroupName = NAME_None;
	FVector BoneOffset = FVector::ZeroVector;
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;

	virtual void Tick(const float DeltaTime) {}

	FVector GetWorldPosition(const FVector& FromPosition) const;
	FVector GetWorldPosition() const { return GetWorldPosition(Position); }
	FVector GetWorldPosition(const FPoseEffector& FromEffector) const { return GetWorldPosition(FromEffector.Position); }
	
	FQuat GetWorldRotation(const FQuat& FromRotation) const;
	FQuat GetWorldRotation() const { return GetWorldRotation(Rotation.Quaternion()); }
	FQuat GetWorldRotation(const FPoseEffector& FromEffector) const { return GetWorldRotation(FromEffector.Rotation.Quaternion()); }

	virtual ~FControlledBone() = default;
};
