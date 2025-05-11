#include "DragonAnimInstance.h"

#include <string>

#include "Constraint.h"
#include "ControlledLeg.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Poses/DragonIdlePose.h"
#include "Poses/DragonTrotPose.h"
#include "Poses/DragonWalkPose.h"
#include "Poses/DragonJumpPose.h"

/**
 * @brief Init
 */
void UDragonAnimInstance::NativeInitializeAnimation()
{
	HipRotation = FRotator();
	LegPositions = TArray<FVector>();
	LegRotations = TArray<FRotator>();
	for (int i = 0; i < 4; i++)
	{
		LegPositions.Add(FVector(0.0f, 0.0f, 0.0f));
		LegRotations.Add(FRotator(0.0f, 0.0f, 0.0f));
	}

	ControlledBody = new FControlledBone();
	ControlledHips = new FControlledBone();
	ControlledLegs = TArray<FControlledLeg*>();
	BackLeftLeg = new FControlledLeg(this, 0);
	BackRightLeg = new FControlledLeg(this, 1);
	ControlledLegs.Add(BackLeftLeg);
	ControlledLegs.Add(BackRightLeg);

	StateMachine = new FDragonAnimStateMachine(
		new FDragonIdlePose(this),
		new FDragonWalkPose(this),
		new FDragonTrotPose(this),
		new FDragonJumpPose(this)
	);
}

/**
 * @brief Every frame, update the animation state and leg positions.
 */
void UDragonAnimInstance::NativeUpdateAnimation(const float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	const auto OwningActor = Cast<AMainCharacter>(GetOwningActor());
	if (!OwningActor)
		return;
	
	StateMachine->Tick(DeltaTime, OwningActor);

	// Apply body driver
	auto CumulativeEffector = FPoseEffector(ControlledBody->Position, ControlledBody->Rotation);
	for (const auto PoseDriver : StateMachine->PoseDrivers)
	{
		CumulativeEffector = PoseDriver->ToBodyEffector(CumulativeEffector, ControlledBody);
	}
	ControlledBody->Position = CumulativeEffector.Position;
	ControlledBody->Rotation = CumulativeEffector.Rotation;
	GetSkelMeshComponent()->SetRelativeLocation(ControlledBody->Position);
	GetSkelMeshComponent()->SetRelativeRotation(ControlledBody->Rotation);

	// Apply hips driver
	CumulativeEffector = FPoseEffector(ControlledHips->Position, ControlledHips->Rotation);
	for (const auto PoseDriver : StateMachine->PoseDrivers)
	{
		CumulativeEffector = PoseDriver->ToHipsEffector(CumulativeEffector, ControlledHips);
	}
	ControlledHips->Position = CumulativeEffector.Position;
	ControlledHips->Rotation = CumulativeEffector.Rotation;

	// SetBoneOffset("Tail_001", FVector(0, 0, 0), FRotator(10, 0, 0));
	SetBoneOffset("Root", "Tail_001", ControlledHips->Position, ControlledHips->Rotation);

	// Apply leg drivers
	for (int i = 0; i < ControlledLegs.Num(); i++)
	{
		const auto Leg = ControlledLegs[i];

		CumulativeEffector = FPoseEffector(Leg->Position, Leg->Rotation);
		for (const auto PoseDriver : StateMachine->PoseDrivers)
		{
			CumulativeEffector = PoseDriver->ToLegEffector(CumulativeEffector, ControlledLegs[i]);
		}

		// Merge the positions and rotations of all effectors
		Leg->Position = CumulativeEffector.Position;
		Leg->Rotation = CumulativeEffector.Rotation;
		LegPositions[i] = Leg->Position;
		LegRotations[i] = Leg->Rotation;
	}
}

void UDragonAnimInstance::SetBoneOffset(const FName ParentBone, const FName ChildName, FVector Position, FRotator Rotation)
{
	FRotator FixedRotation = FRotator(Rotation.Yaw, Rotation.Roll, -Rotation.Pitch + 90);
	USkeletalMeshComponent* SkelComp = GetSkelMeshComponent();
	const auto Constraints = GetSkelMeshComponent()->Constraints;
	for (FConstraintInstance* Constraint : Constraints)
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
