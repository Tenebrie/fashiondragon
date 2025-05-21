#include "DragonAnimInstance.h"

#include <string>

#include "Adapters/DragonWingPoseAdapter.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "Limbs/ControlledLeg.h"
#include "Poses/DragonFootPlacement/DragonFootPlacementPose.h"
#include "Poses/DragonIdle/DragonIdlePose.h"
#include "Poses/DragonTrot/DragonTrotPose.h"
#include "Poses/DragonWalk/DragonWalkPose.h"
#include "Poses/DragonJump/DragonJumpPose.h"
#include "Poses/DragonMomentum/DragonMomentumPose.h"
#include "Poses/DragonRandomSway/DragonRandomSwayPose.h"

/**
 * @brief Init
 */
void UDragonAnimInstance::NativeInitializeAnimation()
{
	LegPositions = TArray<FVector>();
	LegRotations = TArray<FRotator>();
	for (int i = 0; i < 4; i++)
	{
		LegPositions.Add(FVector(0.0f, 0.0f, 0.0f));
		LegRotations.Add(FRotator(0.0f, 0.0f, 0.0f));
	}
	WingPoseAdapter = new FDragonWingPoseAdapter(this);

	ControlledBody = TArray<FControlledBone*>();
	ControlledHips = TArray<FControlledBone*>();
	
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
		new FDragonRandomSwayPose(this),
		new FDragonMomentumPose(this),
		new FDragonFootPlacementPose(this)
	);
}

/**
 * @brief Every frame, update the animation state and leg positions.
 */
void UDragonAnimInstance::NativeUpdateAnimation(const float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (DeltaTime <= KINDA_SMALL_NUMBER) { return; }

	const auto OwningActor = Cast<AMainCharacter>(GetOwningActor());
	if (!OwningActor)
		return;

	StateMachine->Tick(DeltaTime, OwningActor);

	// Apply body driver
	auto CumulativeEffector = FPoseEffector();
	for (int i = 0; i < ControlledBody.Num(); i++)
	{
		auto LocalEffector = FPoseEffector(ControlledBody[i]->Position, ControlledBody[i]->Rotation);
		
		for (const auto PoseDriver : StateMachine->PoseDrivers)
		{
			LocalEffector = PoseDriver->ToBodyEffector(LocalEffector, ControlledBody[i], DeltaTime);
		}
		ControlledBody[i]->Position = LocalEffector.Position;
		ControlledBody[i]->Rotation = LocalEffector.Rotation;
		CumulativeEffector.Position += LocalEffector.Position;
		CumulativeEffector.Rotation += LocalEffector.Rotation;
	}
	GetSkelMeshComponent()->SetRelativeLocation(CumulativeEffector.Position);
	GetSkelMeshComponent()->SetRelativeRotation(CumulativeEffector.Rotation);

	// Apply hips driver
	CumulativeEffector = FPoseEffector();
	for (int i = 0; i < ControlledHips.Num(); i++)
	{
		auto LocalEffector = FPoseEffector(ControlledHips[i]->Position, ControlledHips[i]->Rotation);
		
		for (const auto PoseDriver : StateMachine->PoseDrivers)
		{
			LocalEffector = PoseDriver->ToHipsEffector(LocalEffector, ControlledHips[i], DeltaTime);
		}
		ControlledHips[i]->Position = LocalEffector.Position;
		ControlledHips[i]->Rotation = LocalEffector.Rotation;
		CumulativeEffector.Position += LocalEffector.Position;
		CumulativeEffector.Rotation += LocalEffector.Rotation;
	}
	SetBoneOffset("Hip", "Tail_001", CumulativeEffector.Position, CumulativeEffector.Rotation);

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

	for (FControlledBone* Body : ControlledBody)
		Body->Tick(DeltaTime);
	for (FControlledBone* Hips : ControlledHips)
		Hips->Tick(DeltaTime);
	for (FControlledLeg* Leg : ControlledLegs)
		Leg->Tick(DeltaTime);
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

AMainCharacter* UDragonAnimInstance::GetCharacter() const
{
	return Cast<AMainCharacter>(GetOwningActor());
}
