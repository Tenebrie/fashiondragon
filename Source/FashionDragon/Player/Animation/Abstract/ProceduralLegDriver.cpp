#include "ProceduralLegDriver.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "Curves/BezierUtilities.h"

void FProceduralLegDriver::SetWalkingState(const ELegWalkingState NewState, const bool KeepCycle)
{
	WalkingState = NewState;
	VisualCyclePosition = 0.0f;
	if (!KeepCycle)
	{
		CyclePosition = 0.0f;
		CycleDuration = GetTargetPosition().Duration;
	}
	PositionFrom = Leg->Position;
	RotationFrom = Leg->Rotation;
}

/**
 * Attempts to find the ground directly under the leg and mark it as the locked position.
 * @return Whether ground is detected
 */
bool FProceduralLegDriver::LockToWorldGround(const bool KeepCycle)
{
	SetWalkingState(ELegWalkingState::Planted, KeepCycle);
	SetWalkingState(ELegWalkingState::SeekingGround, true);
	
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform();
	const auto PlantedPosition = Leg->GetPlantedWorldPosition(300.0f);

	const FRotator LockedRotation = FRotator(Leg->Rotation.Pitch, Leg->Rotation.Yaw, 0);
	if (!PlantedPosition.GroundHit)
	{
		SetWalkingState(ELegWalkingState::Raised, true);
		LockedWorldPosition = Transform.TransformPosition(Leg->Position);
		LockedWorldRotation = Transform.TransformRotation(LockedRotation.Quaternion()).Rotator();
		return false;
	}

	// if (PlantedPosition.DeltaPosition.Size() > 5.0f)
		// SetWalkingState(ELegWalkingState::SeekingGround, true);
	
	LockedWorldPosition = Transform.TransformPosition(Leg->Position + PlantedPosition.DeltaPosition);
	LockedWorldRotation = Transform.TransformRotation(LockedRotation.Quaternion()).Rotator();
	return true;
}

void FProceduralLegDriver::SnapToLockedPosition()
{
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().Inverse();
	DesiredPosition = Transform.TransformPosition(LockedWorldPosition);
	DesiredRotation = Transform.TransformRotation(FQuat(LockedWorldRotation)).Rotator();
}

void FProceduralLegDriver::Tick(const float DeltaTime)
{
	const auto StateData = AlignPoseToInputDirection(GetTargetPosition());
	const auto AdvanceTime = DeltaTime * StateData.PlaybackSpeed;
	
	CyclePosition = std::min(CycleDuration, CyclePosition + AdvanceTime);
	VisualCyclePosition = std::min(StateData.Duration, VisualCyclePosition + AdvanceTime);

	// Check if state machine needs to be advanced
	if (CyclePosition >= CycleDuration)
	{
		CyclePosition = CycleDuration;
		AdvanceState();
	}

	const auto GroundData = Leg->GetPlantedWorldPosition(1.0f);
	if (WalkingState == ELegWalkingState::SeekingGround && GroundData.GroundHit)
	{
		LockToWorldGround(true);
		SetWalkingState(ELegWalkingState::Planted, true);
	}
	
	if (WalkingState == ELegWalkingState::Planted)
	{
		SnapToLockedPosition();
	}
	else
	{
		RecalculatePose(AdvanceTime);
	}
}

void FProceduralLegDriver::RecalculatePose([[maybe_unused]] const float DeltaTime)
{
	// Calculate the desired position of the current state
	const auto Direction = GetTargetPosition();
	auto Duration = Direction.Duration;
	if (FMath::Abs(Duration) < 0.001f)
		Duration = 0.001f;

	auto TargetPosition = Direction.TargetPosition;
	auto TargetRotation = Direction.TargetRotation;
	if (WalkingState == ELegWalkingState::SeekingGround)
	{
		const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().Inverse();
		TargetPosition = Transform.TransformPosition(LockedWorldPosition);
		TargetRotation = Transform.TransformRotation(FQuat(LockedWorldRotation)).Rotator();
	}
	
	const auto PositionCurve = UE::CubicBezier::Eval(
			PositionFrom,
			PositionFrom + Direction.StartArticulationPosition,
			TargetPosition + Direction.EndArticulationPosition,
			TargetPosition,
			VisualCyclePosition / Duration
		);
	
	const auto RotationCurve = UE::CubicBezier::Eval(
		FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll),
		FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll) + Direction.StartArticulationRotation,
		FVector(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll) + Direction.EndArticulationRotation,
		FVector(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll),
		VisualCyclePosition / Duration
	);

	DesiredPosition = PositionCurve;
	DesiredRotation = FRotator(RotationCurve.X, RotationCurve.Y, RotationCurve.Z);
}

FDragonWalkStateData FProceduralLegDriver::GetTargetPosition() const
{
	return AlignPoseToInputDirection(GetRawWalkStateData());
}

FDragonWalkStateData FProceduralLegDriver::AlignPoseToInputDirection(FDragonWalkStateData PoseData) const
{
	const auto Data = FDragonWalkStateData(PoseData);
	Data.TargetPosition = FVector(Data.TargetPosition.X * Leg->MirrorScalar, Data.TargetPosition.Y, Data.TargetPosition.Z);
	Data.TargetRotation.Yaw *= Leg->MirrorScalar;
	Data.StartArticulationPosition.X *= Leg->MirrorScalar;
	Data.EndArticulationPosition.X *= Leg->MirrorScalar;
	
	Data.TargetPosition = RotateVectorToInputRotation(Data.TargetPosition);
	
	const auto InputRotation = FMath::Abs(FMath::Cos(GetInputRotation()));
	const auto OriginalZ = Data.TargetPosition.Z;
	const auto StepScale = (InputRotation + 2.0f) / 3.0f;
	Data.TargetPosition *= StepScale;
	Data.TargetPosition.Z = OriginalZ;
	Data.PlaybackSpeed = 1.0f / StepScale;

	if (WalkingState == ELegWalkingState::Stepping || WalkingState == ELegWalkingState::Relaxed)
	{
		const auto Planted = Leg->GetPlantedWorldPosition(Data.TargetPosition, Data.TargetRotation, 150.0f);
		if (Planted.GroundHit)
			Data.TargetPosition += Planted.DeltaPosition;
	}
	return Data;
}

void FProceduralLegDriver::SyncStateFrom(const FProceduralLegDriver* TargetDriver)
{
	WalkingState = TargetDriver->WalkingState;
	CyclePosition = TargetDriver->CyclePosition;
	VisualCyclePosition = TargetDriver->VisualCyclePosition;
	LockedWorldPosition = TargetDriver->LockedWorldPosition;
	LockedWorldRotation = TargetDriver->LockedWorldRotation;
	DesiredPosition = TargetDriver->GetDesiredPosition();
	DesiredRotation = TargetDriver->GetDesiredRotation();
	CycleDuration = GetTargetPosition().Duration;
}

FPoseEffector FProceduralLegDriver::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto State = GetTargetPosition();

	const auto TargetPosition = DesiredPosition;
	const auto TargetRotation = DesiredRotation;
	auto LinearSpeed = State.LinearForce * Context.BlendAlpha * Context.DeltaTime;
	auto RotationSpeed = State.AngularForce * Context.BlendAlpha;

	if (WalkingState == ELegWalkingState::Planted)
	{
		LinearSpeed *= 1.f;
		RotationSpeed *= 0.1f;
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
