#include "DragonAnimInstance.h"

#include <string>

#include "Adapters/DragonNeckPoseAdapter.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "Limbs/ControlledLeg.h"
#include "Limbs/ControlledWing.h"
#include "Poses/CapsuleCompensation/CapsuleCompensationPose.h"
#include "Poses/DragonFlight/DragonFlightPose.h"
#include "Poses/DragonFootPlacement/DragonFootPlacementPose.h"
#include "Poses/DragonIdle/DragonIdlePose.h"
#include "Poses/DragonTrot/DragonTrotPose.h"
#include "Poses/DragonWalk/DragonWalkPose.h"
#include "Poses/DragonJump/DragonJumpPose.h"
#include "Poses/DragonMomentum/DragonMomentumPose.h"
#include "Poses/DragonRandomSway/DragonRandomSwayPose.h"
#include "Poses/DragonNull/DragonNullPose.h"
#include "Poses/DragonSprint/DragonSprintPose.h"

/**
 * @brief Init
 */
void UDragonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	HandTransforms = TArray<FTransform>();
	LegPositions = TArray<FVector>();
	LegRotations = TArray<FRotator>();
	WingEffectors = TArray<FPoseWingEffector>();
	for (int i = 0; i < 2; i++)
	{
		HandTransforms.Add(FTransform());
		LegPositions.Add(FVector(0.0f, 0.0f, 0.0f));
		LegRotations.Add(FRotator(0.0f, 0.0f, 0.0f));
		WingEffectors.Add(FPoseWingEffector());
	}

	// Simple groups
	ControlledRoot = TFControlledBoneGroup("Root", new FControlledBone(this));
	ControlledHead = TFControlledBoneGroup("Head", new FControlledBone(this));
	ControlledBody = TFControlledBoneGroup("Body", new FControlledBone(this));
	ControlledHips = TFControlledBoneGroup("Hips", new FControlledBone(this));

	// Hands
	LeftHand = TFControlledBoneGroup("LeftHand", new FControlledBone(this));
	RightHand = TFControlledBoneGroup("RightHand", new FControlledBone(this));
	
	ControlledHands = TArray<TFControlledBoneGroup<FControlledBone>*>();
	ControlledHands.Add(&LeftHand);
	ControlledHands.Add(&RightHand);

	// Legs
	BackLeftLeg = TFControlledBoneGroup("LeftLeg", new FControlledLeg(
		this,
		FVector(-125.883428, -108.491488, -323.843154),
				0));
	BackRightLeg = TFControlledBoneGroup("RightLeg", new FControlledLeg(
		this,
		FVector(-125.883428, 108.491488, -323.843154),
				1));
	
	ControlledLegs = TArray<TFControlledBoneGroup<FControlledLeg>*>();
	ControlledLegs.Add(&BackLeftLeg);
	ControlledLegs.Add(&BackRightLeg);

	// Wings
	LeftWing = TFControlledBoneGroup("LeftWing", new FControlledWing(this, 0));
	RightWing = TFControlledBoneGroup("RightWing", new FControlledWing(this, 1));
	
	ControlledWings = TArray<TFControlledBoneGroup<FControlledWing>*>();
	ControlledWings.Add(&LeftWing);
	ControlledWings.Add(&RightWing);

	// State machine
	StateMachine = MakeUnique<FDragonAnimStateMachine>(
		new FDragonNullPose(this),
		new FDragonIdlePose(this),
		new FDragonWalkPose(this),
		new FDragonTrotPose(this),
		new FDragonSprintPose(this),
		new FDragonJumpPose(this),
		new FDragonFlightPose(this),
		new FDragonRandomSwayPose(this),
		new FDragonMomentumPose(this),
		new FCapsuleCompensationPose(this),
		new FDragonFootPlacementPose(this)
	);
}

/**
 * @brief Every frame, update the animation state and leg positions.
 */
void UDragonAnimInstance::NativeUpdateAnimation(const float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!GetWorld()->IsGameWorld()) { return; }
	if (DeltaTime <= KINDA_SMALL_NUMBER) { return; }

	const auto OwningActor = Cast<AMainCharacter>(GetOwningActor());
	if (!OwningActor)
		return;

	StateMachine->Tick(DeltaTime, OwningActor);

	// Apply root driver
	auto Effector = ControlledRoot.MakeEffector(StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);
	Effector = ControlledRoot.MakePostProcessEffector(Effector, StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);
	GetSkelMeshComponent()->SetRelativeLocation(Effector.Position);
	GetSkelMeshComponent()->SetRelativeRotation(Effector.Rotation);

	// Apply head driver
	Effector = ControlledHead.MakeEffector(StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);
	Effector = ControlledHead.MakePostProcessEffector(Effector, StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);
	const TArray NeckTransforms = FDragonNeckPoseAdapter::EffectorToTransforms(Effector);
	HeadTransform = NeckTransforms[0];
	UpperNeckTransform = NeckTransforms[1];
	LowerNeckTransform = NeckTransforms[2];

	// Apply body driver
	Effector = ControlledBody.MakeEffector(StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);
	Effector = ControlledBody.MakePostProcessEffector(Effector, StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);
	constexpr float FrontTransformFraction = 0.3f;
	constexpr float BackTransformFraction = 1.0f - FrontTransformFraction;
	const FRotator FrontRotation = FRotator(
		Effector.Rotation.Pitch * FrontTransformFraction,
		Effector.Rotation.Yaw * FrontTransformFraction,
		Effector.Rotation.Roll * FrontTransformFraction);
	const FRotator BackRotation = FRotator(
		Effector.Rotation.Pitch * BackTransformFraction,
		Effector.Rotation.Yaw * BackTransformFraction,
		Effector.Rotation.Roll * BackTransformFraction);
	SpineFrontTransform = FTransform(FrontRotation, Effector.Position * FrontTransformFraction);
	SpineBackTransform = FTransform(BackRotation, Effector.Position * BackTransformFraction);

	// Apply hips driver
	Effector = ControlledHips.MakeEffector(StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);
	Effector = ControlledHips.MakePostProcessEffector(Effector, StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);
	constexpr float HipTransformFraction = 0.35f;
	constexpr float TailTransformFraction = 1.0f - HipTransformFraction;
	const FRotator HipRotation = FRotator(
		Effector.Rotation.Pitch * HipTransformFraction,
		Effector.Rotation.Yaw * HipTransformFraction,
		Effector.Rotation.Roll * HipTransformFraction);
	HipTransform = FTransform(HipRotation, Effector.Position * HipTransformFraction);

	const FRotator TailRotation = FRotator(
		Effector.Rotation.Pitch * TailTransformFraction,
		Effector.Rotation.Yaw * TailTransformFraction,
		Effector.Rotation.Roll * TailTransformFraction);
	SetPhysicalBoneOffset("Hip", "Tail_001", Effector.Position * TailTransformFraction, TailRotation);

	// Apply hand drivers
	for (int i = 0; i < ControlledHands.Num(); i++)
	{
		Effector = ControlledHands[i]->MakeEffector(StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);
		Effector = ControlledHands[i]->MakePostProcessEffector(Effector, StateMachine->PoseDrivers, &FProceduralPose::ToBoneEffector, DeltaTime);

		HandTransforms[i] = FTransform(Effector.Rotation, Effector.Position);
	}

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
		auto WingEffector = WingGroup->MakeWingEffector(StateMachine->PoseDrivers, &FProceduralPose::ToWingEffector, DeltaTime);
		WingEffector = WingGroup->MakePostProcessWingEffector(WingEffector, StateMachine->PoseDrivers, &FProceduralPose::ToWingEffector, DeltaTime);

		WingEffectors[i] = WingEffector;
	}
	const float PhysicsWeight = FMath::Clamp((1.0f - WingEffectors[0].RestFoldState) * 5.0f, 0.0f, 1.0f);
	GetCharacter()->DragonMesh->SetAllBodiesBelowPhysicsBlendWeight(TEXT("Back_L"), PhysicsWeight, true, true);
	GetCharacter()->DragonMesh->SetAllBodiesBelowPhysicsBlendWeight(TEXT("Back_R"), PhysicsWeight, true, true);

	ControlledRoot.Tick(DeltaTime);
	ControlledHead.Tick(DeltaTime);
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

void UDragonAnimInstance::SetPhysicalBoneOffset(const FName ParentBone, const FName ChildName, const FVector& Position, const FRotator& Rotation) const
{
	const FRotator FixedRotation = FRotator(Rotation.Yaw, -Rotation.Roll, Rotation.Pitch + 90);
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

TFControlledBoneGroup<FControlledBone>* UDragonAnimInstance::GetBoneGroup(const FName& BoneName)
{
	const auto BoneGroups = { &ControlledRoot, &ControlledHead, &ControlledBody, &ControlledHips, &LeftHand, &RightHand };
	for (const auto Group : BoneGroups)
	{
		if (BoneName == Group->GetGroupName())
			return Group;
	}
	return nullptr;
}
