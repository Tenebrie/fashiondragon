#include "AbstractProceduralLegDriver.h"

#include "DragonAnimInstance.h"
#include "Curves/BezierUtilities.h"

void FAbstractProceduralLegDriver::SetWalkingState(const ELegWalkingState NewState, const bool KeepCycle)
{
	WalkingState = NewState;
	VisualCyclePosition = 0.0f;
	if (!KeepCycle)
		CyclePosition = 0.0f;
	PositionFrom = Position;
	RotationFrom = Rotation;
	
	if (NewState == Planted)
	{
		const auto Transform = AnimInstance->GetSkelMeshComponent()->GetComponentTransform();
		LockedWorldPosition = Transform.TransformPosition(Position);
		LockedWorldRotation = Transform.TransformRotation(FQuat(Rotation)).Rotator();
	}
}

void FAbstractProceduralLegDriver::ApplyWorldLock()
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

	// Calculate the desired position of the current state
	const auto TargetPosition = GetTargetPosition().first;
	const auto TargetRotation = GetTargetPosition().second;
	
	if (WalkingState == ELegWalkingState::Planted)
	{
		ApplyWorldLock();

		// If the leg is stretched too far, disconnect
		if (Position.Size() > 300.0f && Position.Y < 0.0f)
		{
			SetWalkingState(ELegWalkingState::Inertia, true);
		}
	}
	else
	{
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
}

void FAbstractProceduralLegDriver::AdvanceState()
{
	// Default implementation does nothing
}

std::pair<FVector, FRotator> FAbstractProceduralLegDriver::GetTargetPosition() const
{
	return LEG_POSE((0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f));
}
