#include "CapsuleCompensationPose.h"

#include "Components/CapsuleComponent.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

FCapsuleCompensationPose::FCapsuleCompensationPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BoneDrivers = {
		new FCapsuleCompensationDriverRoot(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::CapsuleCompensation)),
	};
}

FPoseEffector FCapsuleCompensationDriverRoot::ToEffector(const FPoseEffector& BaseEffector,
	const FPoseEffectorContext& Context)
{
	FHitResult Hit;
	const FVector Location = AnimInstance->GetCharacter()->GetActorLocation();
	const bool IsWalking = AnimInstance->GetCharacter()->GetMovementComponent()->IsMovingOnGround();
	if (IsWalking)
	{
		AnimInstance->GetWorld()->LineTraceSingleByChannel(Hit, Location, Location + FVector(0, 0, -1000), ECC_Visibility);
	}

	float TargetHeightOffset = 0.0f;
	FVector Normal = FVector::UpVector;
	if (IsWalking && Hit.bBlockingHit)
	{
		Normal = Hit.Normal;
		TargetHeightOffset = AnimInstance->GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - Hit.Distance;
	}

	constexpr float ExtraOffsetValue = 250.0f;
	const FVector TargetOffset = FVector(0, 0, -ExtraOffsetValue) + Normal * ExtraOffsetValue + FVector(0, 0, TargetHeightOffset * 2.0f);

	const FVector Offset = FMath::VInterpTo(LastFrameOffset, TargetOffset, Context.DeltaTime, 5.0f);
	
	LastFrameOffset = Offset;
	return BaseEffector.SetPosition(AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentRotation().RotateVector(Offset));
}
