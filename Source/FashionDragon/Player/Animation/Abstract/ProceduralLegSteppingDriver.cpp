#include "ProceduralLegSteppingDriver.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "Curves/BezierUtilities.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Utils/Utils.h"

void FProceduralLegSteppingDriver::SetWalkingState(const ELegWalkingState NewState)
{
	OnWalkStateChanged.Broadcast(WalkingState, NewState);
	WalkingState = NewState;
	CyclePosition = 0.0f;

	const auto PlantedPos = Leg->GetPlantedWorldPosition(0.0f, 50.0f);
	PositionFrom = Leg->Position + PlantedPos.DeltaPosition;
	RotationFrom = Leg->Rotation + PlantedPos.DeltaRotation.Rotator();
}

/**
 * Attempts to find the ground directly under the leg and mark it as the locked position.
 * @return Whether ground is detected
 */
bool FProceduralLegSteppingDriver::LockToWorldGround()
{
	SetWalkingState(ELegWalkingState::Planted);
	SetWalkingState(ELegWalkingState::SeekingGround);
	
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform();
	const auto PlantedPosition = Leg->GetPlantedWorldPosition(300.0f, 50.0f);

	const FRotator LockedRotation = FRotator(Leg->Rotation.Pitch, Leg->Rotation.Yaw, 0);
	if (!PlantedPosition.GroundHit)
	{
		SetWalkingState(ELegWalkingState::Raised);
		LockedWorldPosition = Transform.TransformPosition(Leg->Position);
		LockedWorldRotation = Transform.TransformRotation(LockedRotation.Quaternion()).Rotator();
		return false;
	}

	LockedWorldPosition = Transform.TransformPosition(Leg->Position + PlantedPosition.DeltaPosition);
	LockedWorldRotation = Transform.TransformRotation(LockedRotation.Quaternion()).Rotator();
	return true;
}

FString FProceduralLegSteppingDriver::GetDebugState() const
{
	return FUtils::EnumToString(WalkingState);
}

void FProceduralLegSteppingDriver::SnapToLockedPosition()
{
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().Inverse();
	DesiredPosition = Transform.TransformPosition(LockedWorldPosition);
	DesiredRotation = Transform.TransformRotation(FQuat(LockedWorldRotation)).Rotator();
}

FDragonWalkStateData FProceduralLegSteppingDriver::GetRawWalkStateData() const
{
	return FDragonWalkStateData();
}

void FProceduralLegSteppingDriver::Tick(const float DeltaTime)
{
	const auto StateData = AlignPoseToInputDirection(GetTargetPosition());
	const auto AdvanceTime = DeltaTime * StateData.PlaybackSpeed;
	FProceduralLegDriver::Tick(AdvanceTime);
	
	CyclePosition = std::min(StateData.Duration, CyclePosition);

	const auto GroundData = Leg->GetPlantedWorldPosition(1.0f);
	if (WalkingState == ELegWalkingState::SeekingGround && GroundData.GroundHit)
	{
		LockToWorldGround();
		SetWalkingState(ELegWalkingState::Planted);
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

void FProceduralLegSteppingDriver::RecalculatePose([[maybe_unused]] const float DeltaTime)
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
			CyclePosition / Duration
		);
	
	const auto RotationCurve = UE::CubicBezier::Eval(
		FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll),
		FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll) + Direction.StartArticulationRotation,
		FVector(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll) + Direction.EndArticulationRotation,
		FVector(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll),
		CyclePosition / Duration
	);

	DesiredPosition = PositionCurve;
	DesiredRotation = FRotator(RotationCurve.X, RotationCurve.Y, RotationCurve.Z);
}

FDragonWalkStateData FProceduralLegSteppingDriver::GetTargetPosition() const
{
	return AlignPoseToInputDirection(GetRawWalkStateData());
}

FDragonWalkStateData FProceduralLegSteppingDriver::AlignPoseToInputDirection(FDragonWalkStateData PoseData) const
{
	const auto Data = FDragonWalkStateData(PoseData);
	Data.TargetPosition = FVector(Data.TargetPosition.X, Data.TargetPosition.Y * Leg->MirrorScalar, Data.TargetPosition.Z);
	Data.TargetRotation.Yaw *= Leg->MirrorScalar;
	Data.StartArticulationPosition.Y *= Leg->MirrorScalar;
	Data.EndArticulationPosition.Y *= Leg->MirrorScalar;
	
	Data.TargetPosition = RotateVectorToInputRotation(Data.TargetPosition);
	
	if (WalkingState == ELegWalkingState::Stepping || WalkingState == ELegWalkingState::Relaxed || WalkingState == ELegWalkingState::Inertia)
	{
		const auto DesiredHeight = Data.TargetPosition.Z;
		Data.TargetPosition.Z = 0.0f;
		const auto Planted = Leg->GetPlantedWorldPosition(Data.TargetPosition, Data.TargetRotation, 150.0f, 50.0f);
		FVector DeltaPosition = FVector(0, 0, -150);
		if (Planted.GroundHit)
			DeltaPosition = Planted.DeltaPosition;
		Data.TargetPosition += DeltaPosition;
		Data.TargetPosition.Z += DesiredHeight;
	}
	
	return Data;
}

void FProceduralLegSteppingDriver::SyncStateFrom(const FProceduralLegSteppingDriver* TargetDriver)
{
	WalkingState = TargetDriver->WalkingState;
	LockedWorldPosition = TargetDriver->LockedWorldPosition;
	LockedWorldRotation = TargetDriver->LockedWorldRotation;
	DesiredPosition = TargetDriver->GetDesiredPosition();
	DesiredRotation = TargetDriver->GetDesiredRotation();
	
	const auto TargetStateData = TargetDriver->GetTargetPosition();

	CyclePosition = TargetDriver->CyclePosition * TargetStateData.Duration / FMath::Max(0.01f, TargetStateData.Duration);
}

FPoseEffector FProceduralLegSteppingDriver::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto State = GetTargetPosition();

	const auto TargetPosition = DesiredPosition;
	const auto TargetRotation = DesiredRotation;
	const auto LinearSpeed = State.LinearForce * Context.BlendAlpha * Context.DeltaTime * State.PlaybackSpeed;
	const auto RotationSpeed = State.AngularForce * Context.BlendAlpha * State.PlaybackSpeed;

	const auto Direction = (TargetPosition - BaseEffector.Position).GetSafeNormal();
	const auto DistanceToMove = std::min(LinearSpeed * 1000.0f, static_cast<float>((TargetPosition - BaseEffector.Position).Size()));
	
	auto NewRotation = BaseEffector.Rotation;
	if (RotationSpeed > 0)
		NewRotation = FMath::RInterpTo(BaseEffector.Rotation, TargetRotation, Context.DeltaTime, RotationSpeed * 360.0f);

	return BaseEffector
		.AddPosition(Direction * DistanceToMove)
		.SetRotation(NewRotation);
}
