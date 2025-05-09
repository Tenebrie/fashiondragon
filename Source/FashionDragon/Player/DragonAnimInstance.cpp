#include "DragonAnimInstance.h"

#include <map>

#include "MainCharacter.h"
#include "Curves/BezierUtilities.h"
#include "GameFramework/CharacterMovementComponent.h"

#define LEG_POSE(Vector, Rotator) std::make_pair(FVector Vector, FRotator Rotator)

// ============================================================================
// Dragon Leg State
// ============================================================================

const enum ELegState
{
	Relaxed = 0,
	Raised = 1,
	Stepping = 3,
	Planted = 4,
	Inertia = 5,
};

class UDragonAnimInstance::FControlledLeg
{
public:
	explicit FControlledLeg(UDragonAnimInstance* AnimInstance, const int Idx):
		AnimInstance(AnimInstance), Idx(Idx), AnimationState(Idle)
	{
	}

private:
	UDragonAnimInstance* AnimInstance;
	int Idx;
	EAnimationState AnimationState;
	ELegState LegState = ELegState::Relaxed;
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	// Current percentage of the animation cycle (logic)
	float CyclePosition = 0.0f;
	// Current percentage of the animation cycle (visual)
	float CurvePosition = 0.0f;

	FVector PositionFrom = FVector(0.0f, 0.0f, 0.0f);
	FRotator RotationFrom = FRotator(0.0f, 0.0f, 0.0f);

	FVector LockedWorldPosition = FVector(0.0f, 0.0f, 0.0f);
	FRotator LockedWorldRotation = FRotator(0.0f, 0.0f, 0.0f);

	std::pair<FVector, FRotator> GetTargetPosition() const
	{
		const std::map<ELegState, std::pair<FVector, FRotator>> LegStateToPosition =
		{
			{ ELegState::Relaxed, LEG_POSE((0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
			{ ELegState::Raised, LEG_POSE((0.0f, 0.0f, 150.0f), (0.0f, 0.0f, 60.0f)) },
			{ ELegState::Planted, LEG_POSE((0.0f, -350.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
			{ ELegState::Stepping, LEG_POSE((0.0f, 500.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
			{ ELegState::Inertia, LEG_POSE((0.0f, -350.0f, 200.0f), (0.0f, 0.0f, 60.0f)) },
		};

		const auto CurrentValue = LegStateToPosition.at(LegState);
		
		if (LegState == Stepping && Cast<AMainCharacter>(AnimInstance->GetOwningActor())->IsSprinting)
		{
			return std::pair(CurrentValue.first * 1.3f, CurrentValue.second);
		}
		return CurrentValue;
	}

	float GetStateDuration() const
	{
		const std::map<ELegState, float> LegStateToDuration =
		{
			{ ELegState::Relaxed, 1.0f },
			{ ELegState::Raised, 1.0f },
			{ ELegState::Planted, 1.0f },
			{ ELegState::Stepping, 1.0f },
			{ ELegState::Inertia, 0.5f },
		};

		return LegStateToDuration.at(LegState);
	}

	void SetState(const ELegState State, const bool KeepCycle = false)
	{
		this->LegState = State;
		CurvePosition = 0.0f;
		if (!KeepCycle)
			CyclePosition = 0.0f;
		PositionFrom = Position;
		RotationFrom = Rotation;

		if (State == Planted)
		{
			const auto Transform = AnimInstance->GetSkelMeshComponent()->GetComponentTransform();
			LockedWorldPosition = Transform.TransformPosition(Position);
			LockedWorldRotation = Transform.TransformRotation(FQuat(Rotation)).Rotator();
		}
	}

	void AdvanceState()
	{
		if (CyclePosition < 1.0f)
			return;

		if (AnimationState == Idle)
		{
			SetState(ELegState::Relaxed);
		}
		if (AnimationState == Walking)
		{
			switch (LegState)
			{
			case ELegState::Relaxed:
				SetState(ELegState::Raised);
				break;
			case ELegState::Raised:
				SetState(ELegState::Stepping);
				break;
			case ELegState::Inertia:
				SetState(ELegState::Stepping);
				break;
			case ELegState::Planted:
				SetState(ELegState::Stepping);
				break;
			case ELegState::Stepping:
				SetState(ELegState::Planted);
				break;
			}
		}
	}
	
public:
	FVector GetLocalPosition() const
	{
		if (LegState == Planted)
		{
			const auto Transform = AnimInstance->GetSkelMeshComponent()->GetComponentTransform().Inverse();
			return Transform.TransformPosition(LockedWorldPosition);
		}
		return Position;
	}
	FRotator GetLocalRotation() const
	{
		if (LegState == Planted)
		{
			const auto Transform = AnimInstance->GetSkelMeshComponent()->GetComponentTransform().Inverse();
			return Transform.TransformRotation(FQuat(LockedWorldRotation)).Rotator();
		}
		return Rotation;
	}
	float GetCyclePosition() const { return CyclePosition; }
	ELegState GetState() const { return LegState; }
	
	void RequestState(const ELegState State)
	{
		SetState(State);
	}
	
	void Tick(const float DeltaTime, const EAnimationState AnimState)
	{
		this->AnimationState = AnimState;

		// Advance time forward. Adjusted by character's movement speed.
		const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
		const auto MovementSpeed = OwningActor->GetVelocity().Size();

		float AdvanceValue = DeltaTime;
		if (AnimState == Walking)
			AdvanceValue += MovementSpeed * 0.001f * DeltaTime;

		CyclePosition = std::min(1.0f, CyclePosition + AdvanceValue);
		CurvePosition = std::min(GetStateDuration(), CurvePosition + AdvanceValue);

		// Check if state machine needs to be advanced
		AdvanceState();

		// Calculate the desired position of the current state
		const auto TargetPosition = GetTargetPosition().first;
		const auto TargetRotation = GetTargetPosition().second;

		auto Articulation = 0.f;
		// If we're stepping forward, add some vertical offset (looks nicer)
		if (LegState == ELegState::Stepping)
		{
			Articulation = 100.f;
			if (Cast<AMainCharacter>(AnimInstance->GetOwningActor())->IsSprinting)
			{
				Articulation = 100.f;
			}
		}
		
		if (LegState == ELegState::Planted)
		{
			Position = GetLocalPosition();
			Rotation = GetLocalRotation();

			// If the leg is stretched too far, disconnect
			if (Position.Size() > 300.0f && Position.Y < 0.0f)
			{
				SetState(ELegState::Inertia, true);
			}
		}
		else
		{
			const auto CurveOffset = FVector(0.0f, 0.0f, Articulation);
			const auto CurrentPositionInCurve = CurvePosition / GetStateDuration();
			const auto PositionCurve = UE::CubicBezier::Eval(
				PositionFrom,
				PositionFrom + CurveOffset,
				TargetPosition + CurveOffset,
				TargetPosition, CurrentPositionInCurve
			);
			Position = PositionCurve;

			const auto RotationOffset = FVector(0.0f, 0.0f, Articulation);
			const auto RotationCurve = UE::CubicBezier::Eval(
				FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll),
				FVector(RotationFrom.Pitch, RotationFrom.Yaw, RotationFrom.Roll) + RotationOffset,
				FVector(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll) + RotationOffset / 4,
				FVector(TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll),
				CurrentPositionInCurve
			);
			Rotation = FRotator(RotationCurve.X, RotationCurve.Y, RotationCurve.Z);
		}
	}
};

// ============================================================================
// Dragon Animation Controller
// ============================================================================

void UDragonAnimInstance::NativeInitializeAnimation()
{
	AnimationState = EAnimationState::Idle;
	
	LegPositions = TArray<FVector>();
	for (int i = 0; i < 4; i++)
	{
		LegPositions.Add(FVector(0.0f, 0.0f, 0.0f));
	}
	
	LegRotations = TArray<FRotator>();
	for (int i = 0; i < 4; i++)
	{
		LegRotations.Add(FRotator(0.0f, 0.0f, 0.0f));
	}
	
	const auto OwningActor = Cast<AMainCharacter>(GetOwningActor());
	ControlledLegs = TArray<FControlledLeg*>();
	// BackLeftPaw
	ControlledLegs.Add(new FControlledLeg(this, 0));
	// BackRightPaw
	ControlledLegs.Add(new FControlledLeg(this, 1));

	ControlledLegs[0]->RequestState(Raised);
}


void UDragonAnimInstance::NativeUpdateAnimation(const float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	const auto OwningActor = Cast<AMainCharacter>(GetOwningActor());
	if (!OwningActor)
		return;
	
	const auto MovementVector = FVector(
		OwningActor->GetLastMovementInputVector().X,
		OwningActor->GetLastMovementInputVector().Y,
		0.0f
	);
	if (MovementVector.Size() > 0 && AnimationState == EAnimationState::Idle)
	{
		SetState(Walking);
	}
	if (MovementVector.Size() == 0 && AnimationState == EAnimationState::Walking)
	{
		SetState(Idle);
	}

	for (int i = 0; i < ControlledLegs.Num(); i++)
	{ 
		const auto Leg = ControlledLegs[i];
		Leg->Tick(DeltaTime, AnimationState);
	}

	UpdateWalkingBobCycle();

	for (int i = 0; i < ControlledLegs.Num(); i++)
	{ 
		const auto Leg = ControlledLegs[i];
		LegPositions[i] = Leg->GetLocalPosition() - FVector(0.0f, 0.0f, WalkingBobCycle);
		LegRotations[i] = Leg->GetLocalRotation();
	}
}

void UDragonAnimInstance::UpdateWalkingBobCycle()
{
	if (AnimationState != Walking)
	{
		WalkingBobCycle = 15.0f;
		return;
	}
	
	FControlledLeg* Leg;
	if (ControlledLegs[1]->GetState() == ELegState::Planted)
		Leg = ControlledLegs[1];
	else if (ControlledLegs[0]->GetState() == ELegState::Planted)
		Leg = ControlledLegs[0];
	else
	{
		return;
	}

	const auto OffsetFromOrigin = std::min(1.0, Leg->GetLocalPosition().Size() / 300.0f);
	// Print offset
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
		FString::Printf(TEXT("Offset: %s"), *FString::SanitizeFloat(OffsetFromOrigin)));
	
	WalkingBobCycle = (1.0f - OffsetFromOrigin * 2.0f) * 15.0f;
}

void UDragonAnimInstance::SetState(const EAnimationState State)
{
	this->AnimationState = State;

	if (State == Walking)
	{
		ControlledLegs[0]->RequestState(Stepping);
		ControlledLegs[1]->RequestState(Planted);
	}
	if (State == Idle)
	{
		ControlledLegs[0]->RequestState(Relaxed);
		ControlledLegs[1]->RequestState(Relaxed);
	}
}
