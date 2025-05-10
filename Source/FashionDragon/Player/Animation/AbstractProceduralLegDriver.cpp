#include "AbstractProceduralLegDriver.h"

#include "DragonAnimInstance.h"
#include "Curves/BezierUtilities.h"

void FAbstractProceduralLegDriver::SetWalkingState(const ELegWalkingState NewState, const bool KeepCycle)
{
	WalkingState = NewState;
	VisualCyclePosition = 0.0f;
	if (!KeepCycle)
		CyclePosition = 0.0f;
	PositionFrom = Leg->Position;
	RotationFrom = Leg->Rotation;
}

void FAbstractProceduralLegDriver::LockRealWorldPosition()
{
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetComponentTransform();
	LockedWorldPosition = Transform.TransformPosition(Leg->Position);
	LockedWorldRotation = Transform.TransformRotation(FQuat(Leg->Rotation)).Rotator();
}

void FAbstractProceduralLegDriver::LockTargetWorldPosition()
{
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetComponentTransform();
	const auto Target = GetTargetPosition();
	LockedWorldPosition = Transform.TransformPosition(Target.first);
	LockedWorldRotation = Transform.TransformRotation(FQuat(Target.second)).Rotator();
}

void FAbstractProceduralLegDriver::SnapToLockedPosition()
{
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetComponentTransform().Inverse();
	Position = Transform.TransformPosition(LockedWorldPosition);
	Rotation = Transform.TransformRotation(FQuat(LockedWorldRotation)).Rotator();
}

FAbstractProceduralLegDriver::FAbstractProceduralLegDriver(UDragonAnimInstance* AnimInstance,
	FControlledLeg* ControlledLeg):
	AnimInstance(AnimInstance), Leg(ControlledLeg)
{}

void FAbstractProceduralLegDriver::Tick(const float DeltaTime)
{
	CyclePosition = std::min(1.0f, CyclePosition + DeltaTime);
	VisualCyclePosition = std::min(1.0f, VisualCyclePosition + DeltaTime);

	// Check if state machine needs to be advanced
	if (CyclePosition >= 1.0f)
	{
		AdvanceState();
	}
	
	if (WalkingState == ELegWalkingState::Planted)
	{
		SnapToLockedPosition();
	}
	else
	{
		RecalculatePose();
	}
}

void FAbstractProceduralLegDriver::RecalculatePose()
{
	// Calculate the desired position of the current state
	const auto TargetPosition = GetTargetPosition().first;
	const auto TargetRotation = GetTargetPosition().second;
	
	const auto PositionCurve = UE::CubicBezier::Eval(
			PositionFrom,
			PositionFrom + ArticulationPosition,
			TargetPosition + ArticulationPosition,
			TargetPosition, VisualCyclePosition
		);
	Position = PositionCurve;

	const auto RotationCurve = UE::CubicBezier::Eval(
		FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll),
		FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll) + ArticulationRotation,
		FVector(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll) + ArticulationRotation / 4,
		FVector(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll),
		VisualCyclePosition
	);
	Rotation = FRotator(RotationCurve.X, RotationCurve.Y, RotationCurve.Z);
}

void FAbstractProceduralLegDriver::SyncStateFrom(FAbstractProceduralLegDriver* TargetDriver)
{
	WalkingState = TargetDriver->WalkingState;
	CyclePosition = TargetDriver->CyclePosition;
	VisualCyclePosition = TargetDriver->VisualCyclePosition;
	LockedWorldPosition = TargetDriver->LockedWorldPosition;
	LockedWorldRotation = TargetDriver->LockedWorldRotation;
	Position = TargetDriver->GetPosition();
	Rotation = TargetDriver->GetRotation();
	ArticulationPosition = TargetDriver->ArticulationPosition;
	ArticulationRotation = TargetDriver->ArticulationRotation;
}

FPoseEffector FAbstractProceduralLegDriver::ToEffector(const FPoseEffector& BaseEffector, const float BlendAlpha)
{
	const auto PositionDelta = Position - BaseEffector.Position;
	const auto RotationDelta = Rotation - BaseEffector.Rotation;
	const auto Effector = FPoseEffector{};
	Effector.Position = BaseEffector.Position + PositionDelta * BlendAlpha;
	Effector.Rotation = BaseEffector.Rotation + RotationDelta * BlendAlpha;
	return Effector;
}

void FAbstractProceduralLegDriver::AdvanceState()
{
	// Default implementation does nothing
}

std::pair<FVector, FRotator> FAbstractProceduralLegDriver::GetTargetPosition() const
{
	return LEG_POSE((0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f));
}
