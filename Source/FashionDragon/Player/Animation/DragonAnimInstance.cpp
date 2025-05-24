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
#include "Poses/DragonSprint/DragonSprintPose.h"

/**
 * @brief Init
 */
void UDragonAnimInstance::NativeInitializeAnimation()
{
	LegPositions = TArray<FVector>();
	LegRotations = TArray<FRotator>();
	for (int i = 0; i < 2; i++)
	{
		LegPositions.Add(FVector(0.0f, 0.0f, 0.0f));
		LegRotations.Add(FRotator(0.0f, 0.0f, 0.0f));
	}
	WingPoseAdapter = new FDragonWingPoseAdapter(this);

	ControlledBody = TFControlledBoneGroup(new FControlledBone());
	ControlledHips = TFControlledBoneGroup(new FControlledBone());
	
	BackLeftLeg = TFControlledBoneGroup(new FControlledLeg(
		this,
		"Foot_Back_L",
		FVector(99.491356, -147.297687, -331.874477),
				0));
	BackRightLeg = TFControlledBoneGroup(new FControlledLeg(
		this,
		"Foot_Back_R",
		FVector(-99.491317, -147.297662, -331.874409),
				1));
	ControlledLegs = TArray<TFControlledBoneGroup<FControlledLeg>*>();
	ControlledLegs.Add(&BackLeftLeg);
	ControlledLegs.Add(&BackRightLeg);
	
	LeftWing = TFControlledBoneGroup(new FControlledWing(this, 0));
	RightWing = TFControlledBoneGroup(new FControlledWing(this, 1));
	ControlledWings = TArray<TFControlledBoneGroup<FControlledWing>*>();
	ControlledWings.Add(&LeftWing);
	ControlledWings.Add(&RightWing);

	StateMachine = new FDragonAnimStateMachine(
		new FDragonIdlePose(this),
		new FDragonWalkPose(this),
		new FDragonTrotPose(this),
		new FDragonSprintPose(this),
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
	auto Effector = ControlledBody.MakeEffector(StateMachine->PoseDrivers, &FProceduralPose::ToBodyEffector, DeltaTime);
	GetSkelMeshComponent()->SetRelativeLocation(Effector.Position);
	GetSkelMeshComponent()->SetRelativeRotation(Effector.Rotation);

	// Apply hips driver
	Effector = ControlledHips.MakeEffector(StateMachine->PoseDrivers, &FProceduralPose::ToHipsEffector, DeltaTime);
	SetBoneOffset("Hip", "Tail_001", Effector.Position, Effector.Rotation);

	// Apply leg drivers
	for (int i = 0; i < ControlledLegs.Num(); i++)
	{
		Effector = ControlledLegs[i]->MakeEffector(StateMachine->PoseDrivers, &FProceduralPose::ToLegEffector, DeltaTime);
		Effector = ControlledLegs[i]->MakePostProcessEffector(Effector, StateMachine->PoseDrivers, &FProceduralPose::ToLegEffector, DeltaTime);
		
		LegPositions[i] = Effector.Position;
		LegRotations[i] = Effector.Rotation;
	}

	// Apply wing drivers
	for (int i = 0; i < ControlledWings.Num(); i++)
	{
		const auto WingGroup = ControlledWings[i];
		const auto WingEffector = WingGroup->MakeWingEffector(StateMachine->PoseDrivers, &FProceduralPose::ToWingEffector, DeltaTime);

		for (const auto Layer : *WingGroup)
			WingPoseAdapter->ApplyEffector(Layer, WingEffector);
	}

	ControlledBody.Tick(DeltaTime);
	ControlledHips.Tick(DeltaTime);
	for (TFControlledBoneGroup<FControlledLeg>* ControlledLeg : ControlledLegs)
		ControlledLeg->Tick(DeltaTime);
	for (TFControlledBoneGroup<FControlledWing>* ControlledWing : ControlledWings)
		ControlledWing->Tick(DeltaTime);
}

void UDragonAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	StateMachine->NativeBeginPlay();
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
