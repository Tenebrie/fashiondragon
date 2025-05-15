#include "ProceduralLegDriver.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "Curves/BezierUtilities.h"
#include "FashionDragon/DebugTools/QuickDebug.h"

void FProceduralLegDriver::SetWalkingState(const ELegWalkingState NewState, const bool KeepCycle)
{
	WalkingState = NewState;
	VisualCyclePosition = 0.0f;
	if (!KeepCycle)
	{
		CyclePosition = 0.0f;
		CycleDuration = GetTargetPosition().StateDuration;
	}
	PositionFrom = Leg->Position;
	RotationFrom = Leg->Rotation;
}

void FProceduralLegDriver::LockRealWorldPosition()
{
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform();
	LockedWorldPosition = Transform.TransformPosition(Leg->Position);
	LockedWorldRotation = Transform.TransformRotation(FQuat(Leg->Rotation)).Rotator();
}

void FProceduralLegDriver::LockTargetWorldPosition()
{
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform();
	const auto Target = GetTargetPosition();
	LockedWorldPosition = Transform.TransformPosition(Target.TargetPosition);
	LockedWorldRotation = Transform.TransformRotation(FQuat(Target.TargetRotation)).Rotator();
}

void FProceduralLegDriver::SnapToLockedPosition()
{
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().Inverse();
	DesiredPosition = Transform.TransformPosition(LockedWorldPosition);
	DesiredRotation = Transform.TransformRotation(FQuat(LockedWorldRotation)).Rotator();
}

FProceduralLegDriver::FProceduralLegDriver(UDragonAnimInstance* AnimInstance,
	FControlledLeg* ControlledLeg): FProceduralBoneDriver(AnimInstance, ControlledLeg), Leg(ControlledLeg)
{
}

void FProceduralLegDriver::Tick(const float DeltaTime)
{
	const auto Duration = GetTargetPosition().StateDuration;
	CyclePosition = std::min(CycleDuration, CyclePosition + DeltaTime);
	VisualCyclePosition = std::min(Duration, VisualCyclePosition + DeltaTime);

	// Check if state machine needs to be advanced
	if (CyclePosition >= CycleDuration)
	{
		CyclePosition = CycleDuration;
		AdvanceState();
	}
	
	if (WalkingState == ELegWalkingState::Planted)
	{
		SnapToLockedPosition();
	}
	else
	{
		RecalculatePose(DeltaTime);
	}
}

void FProceduralLegDriver::RecalculatePose(const float DeltaTime)
{
	// Calculate the desired position of the current state
	const auto Direction = GetTargetPosition();
	auto Duration = Direction.StateDuration;
	if (FMath::Abs(Duration) < 0.001f)
		Duration = 0.001f;
	
	const auto PositionCurve = UE::CubicBezier::Eval(
			PositionFrom,
			PositionFrom + Direction.StartArticulationPosition,
			Direction.TargetPosition + Direction.EndArticulationPosition,
			Direction.TargetPosition,
			VisualCyclePosition / Duration
		);
	
	const auto RotationCurve = UE::CubicBezier::Eval(
		FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll),
		FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll) + Direction.StartArticulationRotation,
		FVector(Direction.TargetRotation.Pitch, Direction.TargetRotation.Yaw, Direction.TargetRotation.Roll) + Direction.EndArticulationRotation,
		FVector(Direction.TargetRotation.Pitch, Direction.TargetRotation.Yaw, Direction.TargetRotation.Roll),
		VisualCyclePosition / Duration
	);

	DesiredPosition = PositionCurve;
	DesiredRotation = FRotator(RotationCurve.X, RotationCurve.Y, RotationCurve.Z);
}

void FProceduralLegDriver::SyncStateFrom(FProceduralLegDriver* TargetDriver)
{
	WalkingState = TargetDriver->WalkingState;
	CyclePosition = TargetDriver->CyclePosition;
	VisualCyclePosition = TargetDriver->VisualCyclePosition;
	LockedWorldPosition = TargetDriver->LockedWorldPosition;
	LockedWorldRotation = TargetDriver->LockedWorldRotation;
	DesiredPosition = TargetDriver->GetDesiredPosition();
	DesiredRotation = TargetDriver->GetDesiredRotation();
	CycleDuration = GetTargetPosition().StateDuration;
}

FPoseEffector FProceduralLegDriver::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto State = GetTargetPosition();
	
	auto TargetPosition = DesiredPosition;
	auto TargetRotation = DesiredRotation;
	auto LinearSpeed = State.LinearForce * Context.BlendAlpha;
	auto RotationSpeed = State.AngularForce * Context.BlendAlpha;

	if (WalkingState == ELegWalkingState::Planted)
	{
		const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().Inverse();
		LinearSpeed *= 250.f;
		RotationSpeed *= 0.1f;
		TargetPosition = Transform.TransformPosition(LockedWorldPosition);
		TargetRotation = Transform.TransformRotation(FQuat(LockedWorldRotation)).Rotator();
	}
	
	const auto Direction = (TargetPosition - BaseEffector.Position).GetSafeNormal();
	const auto DistanceToMove = std::min(LinearSpeed, static_cast<float>((TargetPosition - BaseEffector.Position).Size()));
	
	auto NewRotation = BaseEffector.Rotation;
	if (RotationSpeed > 0)
		NewRotation = FMath::RInterpTo(BaseEffector.Rotation, TargetRotation, Context.DeltaTime, RotationSpeed);

	return BaseEffector
		.AddPosition(Direction * DistanceToMove)
		.SetRotation(NewRotation);
}

void FProceduralLegDriver::AdvanceState()
{
	// Default implementation does nothing
}

FDragonWalkStateData FProceduralLegDriver::GetTargetPosition() const
{
	return FDragonWalkStateData {};
}
