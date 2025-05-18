#include "DragonAnimInstance.h"

#include <string>

#include "Adapters/DragonWingPoseAdapter.h"
#include "Limbs/ControlledLeg.h"
#include "Poses/DragonIdle/DragonIdlePose.h"
#include "Poses/DragonTrot/DragonTrotPose.h"
#include "Poses/DragonWalk/DragonWalkPose.h"
#include "Poses/DragonJump/DragonJumpPose.h"

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
	WingPoseAdapter = new FDragonWingPoseAdapter(this);

	ControlledBody = new FControlledBone();
	ControlledHips = new FControlledBone();
	
	ControlledLegs = TArray<FControlledLeg*>();
	BackLeftLeg = new FControlledLeg(
		this,
		"Foot_Back_L",
		FVector(99.491356, -147.297687, -331.874477),
				0);
	BackRightLeg = new FControlledLeg(
		this,
		"Foot_Back_R",
		FVector(-99.491317, -147.297662, -331.874409),
				1);
	ControlledLegs.Add(BackLeftLeg);
	ControlledLegs.Add(BackRightLeg);
	
	LeftWing = new FControlledWing(this, 0);
	RightWing = new FControlledWing(this, 1);
	ControlledWings = {
		LeftWing,
		RightWing
	};

	StateMachine = new FDragonAnimStateMachine(
		new FDragonIdlePose(this),
		new FDragonWalkPose(this),
		new FDragonTrotPose(this),
		new FDragonJumpPose(this),
		new FDragonFootPlacementPose(this)
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
		CumulativeEffector = PoseDriver->ToBodyEffector(CumulativeEffector, ControlledBody, DeltaTime);
	}
	ControlledBody->Position = CumulativeEffector.Position;
	ControlledBody->Rotation = CumulativeEffector.Rotation;
	GetSkelMeshComponent()->SetRelativeLocation(ControlledBody->Position);
	GetSkelMeshComponent()->SetRelativeRotation(ControlledBody->Rotation);

	// Apply hips driver
	CumulativeEffector = FPoseEffector(ControlledHips->Position, ControlledHips->Rotation);
	for (const auto PoseDriver : StateMachine->PoseDrivers)
	{
		CumulativeEffector = PoseDriver->ToHipsEffector(CumulativeEffector, ControlledHips, DeltaTime);
	}
	ControlledHips->Position = CumulativeEffector.Position;
	ControlledHips->Rotation = CumulativeEffector.Rotation;

	SetBoneOffset("Hip", "Tail_001", ControlledHips->Position, ControlledHips->Rotation);

	// Apply leg drivers
	for (int i = 0; i < ControlledLegs.Num(); i++)
	{
		const auto Leg = ControlledLegs[i];

		CumulativeEffector = FPoseEffector(Leg->Position, Leg->Rotation);
		for (const auto PoseDriver : StateMachine->PoseDrivers)
			CumulativeEffector = PoseDriver->ToLegEffector(CumulativeEffector, ControlledLegs[i], DeltaTime);

		// Merge the positions and rotations of all effectors
		Leg->Position = CumulativeEffector.Position;
		Leg->Rotation = CumulativeEffector.Rotation;

		CumulativeEffector = FPoseEffector(Leg->Position, Leg->Rotation);
		for (const auto PoseDriver : StateMachine->PoseDrivers)
			CumulativeEffector = PoseDriver->ToPostProcessLegEffector(CumulativeEffector, ControlledLegs[i], DeltaTime);

		LegPositions[i] = CumulativeEffector.Position;
		LegRotations[i] = CumulativeEffector.Rotation;
		Leg->VisualPosition = CumulativeEffector.Position;
		Leg->VisualRotation = CumulativeEffector.Rotation;
	}

	// Apply wing drivers
	for (int i = 0; i < ControlledWings.Num(); i++)
	{
		const auto Wing = ControlledWings[i];

		auto CumulativeWingEffector = FPoseWingEffector(Wing->Flap, Wing->Openness);
		for (const auto PoseDriver : StateMachine->PoseDrivers)
			CumulativeWingEffector = PoseDriver->ToWingEffector(CumulativeWingEffector, ControlledWings[i], DeltaTime);

		Wing->Flap = CumulativeWingEffector.Flap;
		Wing->Openness = CumulativeWingEffector.Openness;

		WingPoseAdapter->ApplyEffector(Wing, CumulativeWingEffector);
	}
}

void UDragonAnimInstance::SetBoneOffset(const FName ParentBone, const FName ChildName, const FVector& Position, const FRotator& Rotation) const
{
	const FRotator FixedRotation = FRotator(Rotation.Yaw, Rotation.Roll, -Rotation.Pitch + 90);
	for (const auto Constraints = GetSkelMeshComponent()->Constraints; FConstraintInstance* Constraint : Constraints)
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
